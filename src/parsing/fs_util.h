#ifndef SIST2_FS_UTIL_H
#define SIST2_FS_UTIL_H

#include "src/sist.h"
#include <openssl/evp.h>

#define CLOSE_FILE(f) if ((f).close != NULL) {(f).close(&(f));};

static int fs_read(struct vfile *f, void *buf, size_t size) {
    if (f->fd == -1) {
        f->sha1_ctx = EVP_MD_CTX_new();
        EVP_DigestInit_ex(f->sha1_ctx, EVP_sha1(), NULL);

        f->fd = open(f->filepath, O_RDONLY);
        if (f->fd == -1) {
            EVP_MD_CTX_free(f->sha1_ctx);
            return -1;
        }
    }

    int ret = (int) read(f->fd, buf, size);

    if (ret != 0 && f->calculate_checksum) {
        f->has_checksum = TRUE;
        safe_digest_update(f->sha1_ctx, (unsigned char *) buf, ret);
    }

    return ret;
}

static void fs_close(struct vfile *f) {
    if (f->fd != -1) {
        EVP_DigestFinal_ex(f->sha1_ctx, f->sha1_digest, NULL);
        EVP_MD_CTX_free(f->sha1_ctx);
        f->sha1_ctx = NULL;
        close(f->fd);
        f->fd = -1;
    }
}

static void fs_reset(struct vfile *f) {
    if (f->fd != -1) {
        lseek(f->fd, 0, SEEK_SET);
    }
}

#endif
