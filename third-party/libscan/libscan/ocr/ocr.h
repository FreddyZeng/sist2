#ifndef OCR_H
#define OCR_H

#include "../scan.h"
#include <tesseract/capi.h>

#define MIN_OCR_SIZE 350
#define MIN_OCR_LEN 10

#define OCR_IS_VALID_BPP(d)                                                    \
  ((d) == 1 || (d) == 2 || (d) == 4 || (d) == 8 || (d) == 16 || (d) == 24 ||   \
   (d) == 32)

typedef void (*ocr_extract_callback_t)(const char *, size_t);

__always_inline static void
ocr_extract_text(const char *tesseract_path, const char *tesseract_lang,
                 const unsigned char *img_buf, const int img_w, const int img_h,
                 const int img_bpp, const int img_stride, const int img_xres,
                 const ocr_extract_callback_t cb) {

  if (img_h <= MIN_OCR_SIZE || img_h <= MIN_OCR_SIZE || img_xres <= 0 ||
      !OCR_IS_VALID_BPP(img_bpp)) {
    return;
  }

  TessBaseAPI *api = TessBaseAPICreate();
  TessBaseAPIInit3(api, tesseract_path, tesseract_lang);

  TessBaseAPISetImage(api, img_buf, img_w, img_h, img_bpp, img_stride);
  TessBaseAPISetSourceResolution(api, img_xres);

  char *text = TessBaseAPIGetUTF8Text(api);
  size_t len = strlen(text);
  if (len >= MIN_OCR_LEN) {
    cb(text, len);
  }

  TessBaseAPIEnd(api);
  TessBaseAPIDelete(api);
}

#endif
