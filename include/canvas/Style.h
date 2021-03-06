#ifndef __STYLE_H__
#define __STYLE_H__

#include "Brushstroke.h"
#include <yaml-cpp/yaml.h>
#include <opencv2/opencv.hpp>

#define CANVAS_SCALE_KEY "canvas_scale"
#define STYLE_LAYERS_KEY "layers"

#define BACKGROUND_KEY "bg"

#define REGEN_WIDTH_KEY "regen_width"
#define AVG_BRUSH_WIDTH_KEY "avg_brush_width"
#define VAR_BRUSH_WIDTH_KEY "var_brush_width"
#define MAX_BRUSH_LENGTH_KEY "max_brush_length"
#define STYLE_OPACITY_KEY "opacity"
#define REGEN_MASK_WIDTH_KEY "regen_mask_width"
#define CLIP_THRESHOLD_KEY "clip_threshold"
#define STRENGTH_THRESHOLD_KEY "strength_threshold"
#define STRENGTH_NEIGHBORHOOD_KEY "strength_neighborhood"
#define VAR_COLOR_KEY "var_color"

#define TEX_PATH_KEY "tex_path"
#define MASK_PATH_KEY "mask_path"
#define TEX_SPACING_KEY "tex_spacing"
#define TEX_JITTER_KEY "tex_jitter"
#define BRUSH_TEXTURE_BLEND "tex_blend"

using namespace cv;

class LayerStyle {
public:
    double regenWidth;
    double avgBrushWidth;
    double varBrushWidth;
    double maxBrushLength;

    double opacity;

    double regenMaskWidth;

    double clipThreshold; 
    double strengthThreshold;
    double strengthNeighborhood;

    std::string maskPath;
    Mat maskImage;

    std::string texPath;
    Mat texImage;
    double texSpacing;
    double texJitter;

    double textureBlend;
    Vec3d colorVariance;

    bool loadTextures(std::string yamlPath);
};

class BackgroundLayerStyle : public LayerStyle {
public:
    bool loadTextures(std::string yamlPath);
};

class CanvasStyle {
public:
    double canvasScale;
    std::vector<LayerStyle> layers;
    BackgroundLayerStyle bgStyle;

    bool loadTextures(std::string yamlPath);
};

namespace YAML {

    template<>
    struct convert<LayerStyle> {
        static Node encode(const LayerStyle& style) {
            Node node;
            node[REGEN_WIDTH_KEY] = style.regenWidth;
            node[AVG_BRUSH_WIDTH_KEY] = style.avgBrushWidth;
            node[VAR_BRUSH_WIDTH_KEY] = style.varBrushWidth;
            node[MAX_BRUSH_LENGTH_KEY] = style.maxBrushLength;
            node[STYLE_OPACITY_KEY] = style.opacity;
            node[REGEN_MASK_WIDTH_KEY] = style.regenMaskWidth;
            node[CLIP_THRESHOLD_KEY] = style.clipThreshold;
            node[STRENGTH_THRESHOLD_KEY] = style.strengthThreshold;
            node[STRENGTH_NEIGHBORHOOD_KEY] = style.strengthNeighborhood;
            node[TEX_PATH_KEY] = style.texPath;
            node[MASK_PATH_KEY] = style.maskPath;
            node[TEX_SPACING_KEY] = style.texSpacing;
            node[TEX_JITTER_KEY] = style.texJitter;
            node[BRUSH_TEXTURE_BLEND] = style.textureBlend;
            node[VAR_COLOR_KEY] = style.colorVariance;
            return node;
        }

        static bool decode(const Node& node, LayerStyle& style) {
            if (!node.IsMap())
                return false;

            style.regenWidth = node[REGEN_WIDTH_KEY].as<double>();
            style.avgBrushWidth = node[AVG_BRUSH_WIDTH_KEY].as<double>();
            style.varBrushWidth = node[VAR_BRUSH_WIDTH_KEY].as<double>();
            style.maxBrushLength = node[MAX_BRUSH_LENGTH_KEY].as<double>();
            style.opacity = node[STYLE_OPACITY_KEY].as<double>();
            style.regenMaskWidth = node[REGEN_MASK_WIDTH_KEY].as<double>();
            style.clipThreshold = node[CLIP_THRESHOLD_KEY].as<double>();
            style.strengthThreshold = node[STRENGTH_THRESHOLD_KEY].as<double>();
            style.strengthNeighborhood = node[STRENGTH_NEIGHBORHOOD_KEY].as<double>();
            style.texPath = node[TEX_PATH_KEY].as<std::string>();
            style.maskPath = node[MASK_PATH_KEY].as<std::string>();
            style.texSpacing = node[TEX_SPACING_KEY].as<double>();
            style.texJitter = node[TEX_JITTER_KEY].as<double>();
            style.textureBlend = node[BRUSH_TEXTURE_BLEND].as<double>();
            style.colorVariance = node[VAR_COLOR_KEY].as<Vec3d>();
            return true;
        }
    };

    template<>
    struct convert<BackgroundLayerStyle> {
        static Node encode(const BackgroundLayerStyle& style) {
            Node node;
            node[STYLE_OPACITY_KEY] = style.opacity;
            node[TEX_PATH_KEY] = style.texPath;
            return node;
        }

        static bool decode(const Node& node, LayerStyle& style) {
            if (!node.IsMap())
                return false;

            style.opacity = node[STYLE_OPACITY_KEY].as<double>();
            style.texPath = node[TEX_PATH_KEY].as<std::string>();

            return true;
        }
    };

    template<>
    struct convert<CanvasStyle> {
        static Node encode(const CanvasStyle& style) {
            Node node;
            node[CANVAS_SCALE_KEY] = style.canvasScale;
            node[STYLE_LAYERS_KEY] = style.layers;
            node[BACKGROUND_KEY] = style.bgStyle;
            return node;
        }

        static bool decode(const Node& node, CanvasStyle& style) {
            if (!node.IsMap())
                return false;

            style.canvasScale = node[CANVAS_SCALE_KEY].as<double>();
            for (int i=0; i<node[STYLE_LAYERS_KEY].size(); i++) {
                style.layers.push_back(node[STYLE_LAYERS_KEY][i].as<LayerStyle>());
            }

            style.bgStyle = node[BACKGROUND_KEY].as<BackgroundLayerStyle>();
            return true;
        }
    };
}

#endif
