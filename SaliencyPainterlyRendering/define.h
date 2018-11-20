#pragma once

#define MAX_DEPTH 13
#define DEF_ATTACH 1
#define DEF_MERGE 2

#define _BGR_R 2
#define _BGR_G 1
#define _BGR_B 0

#define RENDER_MAX 5
#define RENDER_SOBEL 0
#define RENDER_SALIENCY 1
#define RENDER_UNION 2
#define RENDER_TWOPASS_MERGE 3
#define RENDER_TWOPASS_ATTACH 4


#define MAX_Gradient 5
#define Gradient_Sobel 0
#define Gradient_Saliency_G 1 
#define Gradient_Saliency_C 2
#define Gradient_Union 3
#define Gradient_Twopass 4

#define MAX_SALIENCY 7
#define MAX_F_CNT 300
#define MAX_STROKE 1000
#define STROKE_FULL (-1)
#define TIME_STAMP_START 1
#define TIME_STAMP_END  2

#define BEFORE_BETTER 0
#define CHANGED_BETTER 1

#define RUN_THREAD 1


#define BRUSH_ALPHA 1
#define BRUSH_STENCIL 2
#define BRUSH_STENCIL_STR "Stencil"
#define BRUSH_ALPHA_STR "Alpha"

#define SALIENCY_STR_SOBEL "Sobel"
#define MAX_BRUSH 64