#include <Adafruit_NeoPixel.h>

class NeoPixelMatrixAnimator {
 private:
  Adafruit_NeoPixel &strip;
  const int **matrix;
  int columns;
  int rows;

  enum AnimationType {
    NONE,
    WIPE_ON_COLUMN_BEGINNING,
    WIPE_ON_COLUMN_END,
    WIPE_OFF_COLUMN_BEGINNING,
    WIPE_OFF_COLUMN_END,
    WIPE_OUT_COLUMN,
    WIPE_IN_COLUMN,
    WIPE_ON_ROW_BEGINNING,
    WIPE_ON_ROW_END,
    WIPE_OFF_ROW_BEGINNING,
    WIPE_OFF_ROW_END,
    WIPE_OUT_ROW,
    WIPE_IN_ROW,
    FADE_IN,
    FADE_OUT
  };

  AnimationType currentAnimation;
  int animationStep;
  int passes;
  uint32_t color;
  bool isAnimating;

  void clearMatrix() {
    for (int c = 0; c < columns; c++) {
      for (int r = 0; r < rows; r++) {
        if (matrix[c][r] != -1) {
          strip.setPixelColor(matrix[c][r], 0);
        }
      }
    }
    strip.show();
  }

 public:
  NeoPixelMatrixAnimator(Adafruit_NeoPixel &neoStrip, const int **ledMatrix, int cols, int rowsCount)
      : strip(neoStrip), matrix(ledMatrix), columns(cols), rows(rowsCount), currentAnimation(NONE), animationStep(0), passes(0), color(0), isAnimating(false) {}

  void setAnimation(AnimationType animation, uint32_t animColor, int animPasses = 1) {
    clearMatrix();
    currentAnimation = animation;
    animationStep = 0;
    passes = animPasses;
    color = animColor;
    isAnimating = true;
  }

  void update() {
    if (!isAnimating)
      return;

    switch (currentAnimation) {
      case WIPE_ON_COLUMN_BEGINNING:
        for (int r = 0; r < rows; r++) {
          if (matrix[animationStep][r] != -1) {
            strip.setPixelColor(matrix[animationStep][r], color);
          }
        }
        strip.show();
        animationStep++;
        if (animationStep >= columns)
          isAnimating = false;
        break;

      case WIPE_ON_COLUMN_END:
        for (int r = 0; r < rows; r++) {
          if (matrix[columns - animationStep - 1][r] != -1) {
            strip.setPixelColor(matrix[columns - animationStep - 1][r], color);
          }
        }
        strip.show();
        animationStep++;
        if (animationStep >= columns)
          isAnimating = false;
        break;

      case WIPE_OFF_COLUMN_BEGINNING:
        for (int r = 0; r < rows; r++) {
          if (matrix[animationStep][r] != -1) {
            strip.setPixelColor(matrix[animationStep][r], 0);
          }
        }
        strip.show();
        animationStep++;
        if (animationStep >= columns)
          isAnimating = false;
        break;

      case WIPE_OFF_COLUMN_END:
        for (int r = 0; r < rows; r++) {
          if (matrix[columns - animationStep - 1][r] != -1) {
            strip.setPixelColor(matrix[columns - animationStep - 1][r], 0);
          }
        }
        strip.show();
        animationStep++;
        if (animationStep >= columns)
          isAnimating = false;
        break;
      case WIPE_OUT_COLUMN:
        int middleStart = (columns - 1) / 2;
        int middleEnd = columns / 2;
        for (int c = middleStart - animationStep; c <= middleEnd + animationStep; c++) {
          if (c >= 0 && c < columns) {
            for (int r = 0; r < rows; r++) {
              if (matrix[c][r] != -1) {
                strip.setPixelColor(matrix[c][r], 0);
              }
            }
          }
        }
        strip.show();
        animationStep++;
        if (middleStart - animationStep < 0 && middleEnd + animationStep >= columns)
          isAnimating = false;
        break;
      case WIPE_ON_ROW_BEGINNING:
        for (int c = 0; c < columns; c++) {
          if (matrix[c][animationStep] != -1) {
            strip.setPixelColor(matrix[c][animationStep], color);
          }
        }
        strip.show();
        animationStep++;
        if (animationStep >= rows)
          isAnimating = false;
        break;
      case FADE_IN:
        for (int c = 0; c < columns; c++) {
          for (int r = 0; r < rows; r++) {
            if (matrix[c][r] != -1) {
              uint32_t scaledColor = strip.Color(
                  ((color >> 16) & 0xFF) * animationStep / passes,
                  ((color >> 8) & 0xFF) * animationStep / passes,
                  (color & 0xFF) * animationStep / passes);
              strip.setPixelColor(matrix[c][r], scaledColor);
            }
          }
        }
        strip.show();
        animationStep++;
        if (animationStep > passes)
          isAnimating = false;
        break;
      case FADE_OUT:
        for (int c = 0; c < columns; c++) {
          for (int r = 0; r < rows; r++) {
            if (matrix[c][r] != -1) {
              uint32_t scaledColor = strip.Color(
                  ((color >> 16) & 0xFF) * (passes - animationStep) / passes,
                  ((color >> 8) & 0xFF) * (passes - animationStep) / passes,
                  (color & 0xFF) * (passes - animationStep) / passes);
              strip.setPixelColor(matrix[c][r], scaledColor);
            }
          }
        }
        strip.show();
        animationStep++;
        if (animationStep > passes)
          isAnimating = false;
        break;

      default:
        isAnimating = false;
        break;
    }
  }
};
