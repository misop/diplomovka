#pragma once

#define MODEL_MATRIX             0
#define VIEW_MATRIX              1
#define SWAP                     1
#define PROJECTION_MATRIX_INV    1
#define PROJECTION_MATRIX        2
#define NORMAL_MATRIX            3
#define MATERIAL                 4
#define NORMAL_PROJ_MATRIX       4
#define PIXELS_PER_EDGE          5
#define SHADOW_MATRIX            6
#define USE_TOON_SHADING         7
#define SCREEN_HEIGHT            8
#define SCREEN_WIDTH             9
#define DISPLACE                 9

#define SUN                     10
#define POINT1                  11
#define POINT2                  12
#define SUN_COLOR               13

#define TEXEL_WIDTH              3
#define TEXEL_HEIGHT             4
#define SOBEL_X                  5
#define TEXEL_SIZE               3
#define HORIZONTAL               4

#define MVP_MATRIX_STR "MVPmatrix"
#define MODEL_MATRIX_STR "ModelMatrix"
#define VIEW_MATRIX_STR "ViewMatrix"
#define PROJECTION_MATRIX_STR "ProjectionMatrix"
#define MODEL_VIEW_MATRIX_STR "ModelViewMatrix"
#define NORMAL_MATRIX_STR "NormalMatrix"
#define SKINNING_MATRICES_STR "SkinningMatrices"
#define TRANSFORM_MATRICES_STR "TransformMatrices"

#define TESS_LEVEL_INNER_STR "TessLevelInner"
#define TESS_LEVEL_OUTER_STR "TessLevelOuter"

#define AMBIENT_COLOR_STR "AmbientColor"
#define DIFFUSE_COLOR_STR "DiffuseColor"
#define SPECULAR_COLOR_STR "SpecularColor"
#define LIGHT_POSITION_STR "LightPosition"

#define WIREFRAME_STR "Wireframe"
#define THRESHOLD_STR "Threshold"
#define MAX_ID_STR "MaxID"

#define RADIUS_TEXTURE_STR "RadiusesSampler"
#define CENTERS_TEXTURE_STR "CenterSampler"

#define SKINNING_TYPE_STR "SkinningType" 
#define PARAM_STR "param"
#define SKINNING_REF_MATRIX_STR "SkinningMatricesReference"
#define ANIM_PARAM_STR  "AnimInterpolation"
#define SCREEN_HEIGHT_STR "ScreenHeight"