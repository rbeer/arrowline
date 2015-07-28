/**
 * ARROWLINE
 * powerline-like shell prompt generator
 *
 * file: utils.c
 * v0.6.3 / 2015.07.16
 *
 * (c) 2015 Bernd Busse
 * The MIT License (MIT)
 **/

#include <limits.h>
#include <pwd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#ifdef USE_VCS_GIT
    #include <git2.h>
#endif // USE_VCS_GIT

#include "utils.h"

/**
 * ENVIRONMENT HELPER FUNCTIONS AND WRAPPER
 **/

#ifdef USE_VCS_GIT
static const int INDEX_FLAGS = GIT_STATUS_INDEX_NEW | GIT_STATUS_INDEX_MODIFIED | GIT_STATUS_INDEX_DELETED | GIT_STATUS_INDEX_RENAMED | GIT_STATUS_INDEX_TYPECHANGE;
static const int WT_FLAGS = GIT_STATUS_WT_NEW | GIT_STATUS_WT_MODIFIED | GIT_STATUS_WT_DELETED | GIT_STATUS_WT_RENAMED | GIT_STATUS_WT_TYPECHANGE | GIT_STATUS_WT_TYPECHANGE;
#endif // USE_VCS_GIT

int last_exit_status = 0;

/* copy username from environment into buf */
int al_get_username(char* buf, size_t len) {
    struct passwd* pw = getpwuid(geteuid());
    if (pw == NULL) {
        return -1; // error
    }
    strncpy(buf, pw->pw_name, len);
    buf[len-1] = '\0';
    return 0;
}

/* copy hostname from environment into buf */
int al_get_hostname(char* buf, size_t len) {
    if (gethostname(buf, len) != 0) {
        return -1; // error
    }
    buf[len-1] = '\0';
    return 0;
}

/* copy path of users home directory into buf */
int al_get_home(char* buf, size_t len) {
    struct passwd* pw = getpwuid(geteuid());
    if (pw == NULL) {
        return -1; // error
    }
    strncpy(buf, pw->pw_dir, len);
    buf[len-1] = '\0';
    return 0;
}

/* copy path of current working directory into buf */
int al_get_cwd(char* buf, size_t len) {
    getcwd(buf, len);
    if (buf == NULL) {
        return -1; // error
    }
    buf[len-1] = '\0';
    return 0;
}

/* check if session is running over SSH */
int al_is_ssh_connection() {
    char* val = getenv("SSH_CONNECTION");
    if (val != NULL) {
        return 1; // SSH SESSION
    }
    return 0; // NOT running over SSH
}

/* check if user is root */
int al_is_root_session() {
    uid_t uid = getuid();
    if (uid == 0) {
        return 1; // USER IS ROOT
    }
    return 0; // NOT user root
}

/* check if last command has failed */
int al_last_command_failed() {
    if (last_exit_status != 0) {
        return 1; // COMMAND FAILED
    }
    return 0; // NOT failed
}

/* enlarge buffer if not large enough */
void al_resize_char_buffer(char** dest, const char* buf, unsigned int* destlen, int step) {
    if ((strlen(*dest) + strlen(buf) + 1) > *destlen) {
        *destlen = *destlen + step;
        char* newbuf = realloc(*dest, *destlen);
        if (newbuf == NULL) {
            perror("ERROR: can't allocate larger prompt buffer");
            free(*dest);
            exit(EXIT_FAILURE);
        } else {
            *dest = newbuf;
        }
    }
}

/* check if str starts with pre */
int al_string_startswith(const char* str, const char* pre) {
    size_t len = strlen(pre);
    if (strlen(str) < len) {
        return 0; // compare string pre is too long
    }
    return strncmp(str, pre, len) == 0;
}

/* safety wrapper arround strncat */
void al_string_cat(char* dst, const char* str, const int maxlen) {
    strncat(dst, str, maxlen - strlen(dst));
    dst[maxlen-1] = '\0';
}

/* return directory level of path */
int al_get_dir_count(const char* path) {
    int c = 1;
    const char* occ = path;

    if (path == NULL) {
        return -1; // error
    }
    // check if path string is empty: level 0
    if (strlen(path) < 2) {
        return 0;
    }

    for (int i = 0; i < INT_MAX; i++) {
        occ = strchr(occ+1, '/');
        if (occ == NULL) {
            break; // return c
        }
        c++;
    }

    return c;
}

/* write directory name count levels obove path into buf */
int al_get_dir_name(char* dest, size_t len, const char* path, int count) {
    char buf[256];
    char* occ;

    // work on a local copy of path
    strncpy(buf, path, 256);
    buf[255] = '\0';

    for (int i = 0; i <= count; i++) {
        occ = strrchr(buf, '/');
        if (occ == NULL) {
            *dest = '\0';
            return -1;
        }
        *occ = '\0';
    }

    strncpy(dest, occ+1, len);
    dest[len-1] = '\0';

    return 0;
}

#ifdef USE_VCS_GIT
/* open git repository at path if it actually is a git repository */
int al_git_open_repo(const char* path, git_repository** repo) {
    if (git_repository_open_ext(repo, path, 0, NULL) != 0) {
        return -1; // error
    }
    return 0;
}

/* copy name of current git branch into buf */
int al_git_get_branch(char* buf, size_t len, git_repository* repo) {
    const char* branch = NULL;
    git_reference* head = NULL;

    int err = git_repository_head(&head, repo);
    if (err == GIT_EUNBORNBRANCH || err == GIT_ENOTFOUND) {
        branch = NULL;
    } else if (!err) {
        branch = git_reference_shorthand(head);
    } else {
        return -1; // error
    }

    strncpy(buf, branch ? branch : "HEAD", len);
    buf[len-1] = '\0';

    git_reference_free(head);
    return 0;
}

/* check if current git repo is clean or dirty 
 * HACK: INVOKE 'git status' and parse output */
int al_git_is_dirty() {
    FILE *cmd;
    int modified = 0;
    char modified_list[10] = { 0 };

    cmd = popen("git status -s --ignore-submodules --porcelain", "r");

    if (cmd == NULL) {
        return -1; // error
    }

    while (fgets(modified_list, 10, cmd) != NULL) {
        if (sizeof(modified_list) != 0) {
            modified = 1; // found modification
            break;
        }
    }

    if (pclose(cmd) != 0) {
        return -1; // error
    }

    return modified;
}
#endif // USE_VCS_GIT
