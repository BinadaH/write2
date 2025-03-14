#include "line.h"
#include "Vec2.h"
#include <iostream>

Line::~Line() {
	points.clear();
    draw_points.clear();
    press.clear();
}


Line::Line() {
    line_width = 5;
}

void Line::AddPoint(Vec2 point, Camera &cam) {
	points.push_back(cam.ScreenToWorld(point));
}

void Line::AddPressPoint(float _press)
{
    press.push_back(_press);
}

void Line::Draw(ImDrawList* draw_list, Camera &cam)
{
    static std::vector<ImVec2> screen_points;
    screen_points.resize(draw_points.size());

    for (int i = 0; i < draw_points.size(); i++) {
		Vec2 pos = (Vec2)draw_points[i];
        screen_points[i] = cam.WorldToScreen(pos).im();
    }

    if (ImGui::GetIO().KeyCtrl) {
        for (int i = 1; i < points.size(); i++) {
            draw_list->AddLine(ImVec2(points[i].x, points[i].y), ImVec2(points[i - 1].x, points[i - 1].y), IM_COL32(255, 255, 255, 255), line_width);
        }
    }
    else {
        //std::cout << cam.ScaleWorldToScreen(25) << "\n";
        draw_list->AddPolylineVariableWidth(screen_points.data(), screen_points.size(), ImColor(255, 255, 255, 255), ImDrawFlags_None, (int)std::roundf(cam.ScaleWorldToScreen(line_width)), press.data(), press.size());
    }
}

void  UpdateWithExpMovingAverage(std::vector<ImVec2> &draw_points, std::vector<Vec2> &points) {
    draw_points.resize(points.size());
    const float a = 0.8f;
    if (points.empty()) return;

    // Start with the first point as the initial "smoothed" point
    ImVec2 smoothedPoint = points[0].im();
    draw_points[0] = smoothedPoint;

    // Apply EMA for each point starting from the second one
    for (size_t i = 1; i < points.size(); ++i) {
        smoothedPoint.x = a * points[i].x + (1 - a) * smoothedPoint.x;
        smoothedPoint.y = a * points[i].y + (1 - a) * smoothedPoint.y;
        draw_points[i] = smoothedPoint; // Update the point with the smoothed value
    }
}

void Line::ApplySmoothing() {
	UpdateWithExpMovingAverage(draw_points, points);
	return ;
    draw_points.resize(points.size());
    const float a = 0.4f;  // EMA smoothing factor

    if (points.size() < 3) return;  // Loess requires at least 3 points

    // Step 1: Apply Loess smoothing on the points
    std::vector<ImVec2> loessSmoothedPoints;

    // Define window size for Loess (number of points to consider for smoothing)
    const int windowSize = 5;  // Use odd values for a symmetrical window
    const float bandwidth = 2.0f;  // Control the weight of the points (higher = more localized smoothing)

    for (size_t i = 0; i < points.size(); ++i) {
        // Define the window of points around the current point
        int start = std::max(0, int(i) - windowSize / 2);
        int end = std::min(int(points.size()) - 1, int(i) + windowSize / 2);

        // Perform weighted least squares fit
        ImVec2 smoothedPoint = ApplyLoessFit(start, end, i, bandwidth);
        loessSmoothedPoints.push_back(smoothedPoint);
    }

    // Step 2: Apply EMA smoothing over the loess smoothed points
    ImVec2 smoothedPoint = loessSmoothedPoints[0]; // Start with the first loess smoothed point
    draw_points[0] = smoothedPoint;

    // Apply EMA to further smooth the points
    for (size_t i = 1; i < loessSmoothedPoints.size(); ++i) {
        smoothedPoint.x = a * loessSmoothedPoints[i].x + (1 - a) * smoothedPoint.x;
        smoothedPoint.y = a * loessSmoothedPoints[i].y + (1 - a) * smoothedPoint.y;
        draw_points[i] = smoothedPoint;  // Store the final smoothed point
    }
}

Vec2 Line::GetLastSmoothedPoint()
{
    if (draw_points.size() > 0)
        return draw_points.back();
    else
        return Vec2(0, 0);
}

// Apply Loess fit (local weighted regression)
ImVec2 Line::ApplyLoessFit(int start, int end, int current, float bandwidth) {
    // Get points in the window
    std::vector<ImVec2> windowPoints;
    for (int i = start; i <= end; ++i) {
        windowPoints.push_back(points[i].im());
    }

    // Prepare the weighted matrix (weights based on distance)
    std::vector<float> weights;
    for (int i = start; i <= end; ++i) {
        float dist = std::abs(i - current);
        float weight = std::exp(-0.5f * (dist * dist) / (bandwidth * bandwidth));  // Gaussian kernel weight
        weights.push_back(weight);
    }

    // Now perform a weighted linear regression to get the smoothed point
    float sumWeights = 0.0f, sumWeightedX = 0.0f, sumWeightedY = 0.0f;

    for (int i = start; i <= end; ++i) {
        float weight = weights[i - start];
        sumWeights += weight;
        sumWeightedX += weight * windowPoints[i - start].x;
        sumWeightedY += weight * windowPoints[i - start].y;
    }

    // Return the weighted average position as the smoothed point
    float smoothedX = sumWeightedX / sumWeights;
    float smoothedY = sumWeightedY / sumWeights;

    return ImVec2(smoothedX, smoothedY);
}



