
// ChildView.cpp : implementation of the CChildView class
//

#include "pch.h"
#include "framework.h"
#include "Project2.h"
#include "ChildView.h"
#include <algorithm>
#include <thread>
#include <chrono>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
    m_camera.Set(10, 10, 35, 0, 0, 0, 0, 1, 0);
    ResetBoxes();
}

CChildView::~CChildView()
{
}


void CChildView::CreateMesh()
{
    double v[8][4] = { {0, 0, 2, 1}, {2, 0, 2, 1}, {2, 2, 2, 1}, {0, 2, 2, 1},
        {0, 0, 0, 1}, {2, 0, 0, 1}, {2, 2, 0, 1}, {0, 2, 0, 1} };
    double n[6][4] = { {0, 0, 1, 0}, {1, 0, 0, 0}, {0, 0, -1, 0},
    {-1, 0, 0, 0}, {0, 1, 0, 0}, {0, -1, 0, 0} };

    for (int i = 0; i < 8; i++)
        m_mesh.AddVertex(v[i]);

    for (int i = 0; i < 6; i++)
        m_mesh.AddNormal(n[i]);

    m_mesh.AddTriangleVertex(0, 0, -1);
    m_mesh.AddTriangleVertex(1, 0, -1);
    m_mesh.AddTriangleVertex(2, 0, -1);
}


BEGIN_MESSAGE_MAP(CChildView, COpenGLWnd)
	ON_WM_PAINT()
    ON_WM_LBUTTONDOWN()
    ON_WM_RBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_BN_CLICKED(1001, OnSortButtonClicked)
    ON_BN_CLICKED(1002, OnResetOriginalButtonClicked)
    ON_BN_CLICKED(1003, OnSortY_AXISButtonClicked)
    ON_WM_TIMER()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!COpenGLWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}


void CChildView::OnGLDraw(CDC* pDC)
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //
    // Set up the camera
    //

    int width, height;
    GetSize(width, height);
    m_camera.Apply(width, height);

    double totalWidth = mBoxes.size() * 3.0;
    double centerX = totalWidth / 2.0;

    gluLookAt(centerX, 5.0, 30.0,
        centerX, 5.0, 0.0,
        0.0, 1.0, 0.0);

    GLfloat lightpos[] = { 0.5, 2.0, 1.0, 0. };
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);

    //
    // Some standard parameters
    //

    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Cull backfacing polygons
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    // Enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    glPushMatrix();


    int x = -20;

    for (auto& box : mBoxes) {
        glPushMatrix();
        glTranslated(box.currentX, box.offsetY, box.offsetZ);
        glColor3dv(box.color.data());
        box.mesh.Draw();
        glPopMatrix();
    }
    glPopMatrix();
}


/*
* Project-specific functions
*/


CMesh CChildView::CreateBox(double height) {
    CMesh mesh;

    const double w = 2.0;
    const double d = 2.0;

    double x = w / 2.0;
    double z = d / 2.0;
    double yBottom = 0.0;
    double yTop = height;

    mesh.AddVertex(CGrVector(-x, yBottom, z));
    mesh.AddVertex(CGrVector(x, yBottom, z));
    mesh.AddVertex(CGrVector(x, yTop, z));
    mesh.AddVertex(CGrVector(-x, yTop, z));

    mesh.AddVertex(CGrVector(-x, yBottom, -z));
    mesh.AddVertex(CGrVector(x, yBottom, -z));
    mesh.AddVertex(CGrVector(x, yTop, -z));
    mesh.AddVertex(CGrVector(-x, yTop, -z));

    mesh.AddNormal(CGrVector(0, 0, 1));
    mesh.AddNormal(CGrVector(0, 0, -1));
    mesh.AddNormal(CGrVector(0, 1, 0));
    mesh.AddNormal(CGrVector(0, -1, 0));
    mesh.AddNormal(CGrVector(1, 0, 0));
    mesh.AddNormal(CGrVector(-1, 0, 0));

    mesh.AddTexCoord(CGrVector(0, 0, 0));
    mesh.AddTexCoord(CGrVector(1, 0, 0));
    mesh.AddTexCoord(CGrVector(1, 1, 0));
    mesh.AddTexCoord(CGrVector(0, 1, 0));

    mesh.AddTriangleVertex(0, 0, 0); mesh.AddTriangleVertex(1, 0, 1); mesh.AddTriangleVertex(2, 0, 2);
    mesh.AddTriangleVertex(0, 0, 0); mesh.AddTriangleVertex(2, 0, 2); mesh.AddTriangleVertex(3, 0, 3);
    mesh.AddTriangleVertex(5, 1, 0); mesh.AddTriangleVertex(4, 1, 1); mesh.AddTriangleVertex(7, 1, 2);
    mesh.AddTriangleVertex(5, 1, 0); mesh.AddTriangleVertex(7, 1, 2); mesh.AddTriangleVertex(6, 1, 3);
    mesh.AddTriangleVertex(3, 2, 0); mesh.AddTriangleVertex(2, 2, 1); mesh.AddTriangleVertex(6, 2, 2);
    mesh.AddTriangleVertex(3, 2, 0); mesh.AddTriangleVertex(6, 2, 2); mesh.AddTriangleVertex(7, 2, 3);
    mesh.AddTriangleVertex(4, 3, 0); mesh.AddTriangleVertex(5, 3, 1); mesh.AddTriangleVertex(1, 3, 2);
    mesh.AddTriangleVertex(4, 3, 0); mesh.AddTriangleVertex(1, 3, 2); mesh.AddTriangleVertex(0, 3, 3);
    mesh.AddTriangleVertex(1, 4, 0); mesh.AddTriangleVertex(5, 4, 1); mesh.AddTriangleVertex(6, 4, 2);
    mesh.AddTriangleVertex(1, 4, 0); mesh.AddTriangleVertex(6, 4, 2); mesh.AddTriangleVertex(2, 4, 3);
    mesh.AddTriangleVertex(4, 5, 0); mesh.AddTriangleVertex(0, 5, 1); mesh.AddTriangleVertex(3, 5, 2);
    mesh.AddTriangleVertex(4, 5, 0); mesh.AddTriangleVertex(3, 5, 2); mesh.AddTriangleVertex(7, 5, 3);

    return mesh;
}

void CChildView::ResetBoxes() {

    // Clear vector if exists
    /*if (!mBoxes.empty()) {
        mBoxes.clear();
    }*/

    mBoxes.clear();

    // Constant original values
    const vector<int> startingHeights = {5, 7, 3, 1, 4, 6, 2, 10, 8, 9};

    // Create array of meshes

    int currX = 0;
    std::array<double, 3> WHITE = { 1.0, 1.0, 1.0 };

    for (auto height : startingHeights) {
        BoxStruct box;
        box.height = height;
        box.mesh = CreateBox(height);
        box.currentX = currX;
        box.startX = currX;
        box.offsetY = 0;
        box.offsetZ = 0;
        box.color = mUNSORTED;
        mBoxes.push_back(box);
        currX = currX + 3;
    }
}

void CChildView::Swap(int x, int y) {

    if (x == y) {
        return;
    }

    // Set start variables to calculate progress
    mBoxes[x].startX = mBoxes[x].currentX;
    mBoxes[y].startX = mBoxes[y].currentX;

    // Swap the boxes internally
    std::swap(mBoxes[x], mBoxes[y]);

    // Start animation
    m_animating = true;

    // Start timer for thread
    ::SetTimer(this->m_hWnd, 1, 16, nullptr);

    // Wait till timer completes
    while (m_animating) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

int CChildView::Partition(int low, int high) {
    int pivot = mBoxes[high].height;
    mBoxes[high].color = mPIVOT;
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (mBoxes[j].height < pivot)
        {
            i++;
            Swap(i, j);
        }
    }

    Swap(i + 1, high);

    return i + 1;
}

void CChildView::QuickSort(int low, int high) {
    if (low < high)
    {
        int pivotIndex = Partition(low, high);

        // Change pivot color bc it is in it's position
        mBoxes[pivotIndex].color = mSORTED;

        QuickSort(low, pivotIndex - 1);
        QuickSort(pivotIndex + 1, high);
    }
    else if (low == high) {

        // If they are the same position
        // they are sorted
        mBoxes[low].color = mSORTED;
    }
    Invalidate();
}


// Event handlers
void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_camera.MouseDown(point.x, point.y);

    COpenGLWnd::OnLButtonDown(nFlags, point);
}


void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_camera.MouseMove(point.x, point.y, nFlags))
        Invalidate();

    COpenGLWnd::OnMouseMove(nFlags, point);
}


void CChildView::OnRButtonDown(UINT nFlags, CPoint point)
{
    m_camera.MouseDown(point.x, point.y, 2);

    COpenGLWnd::OnRButtonDown(nFlags, point);
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct) {
    if (COpenGLWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    mQuicksortButton.Create(L"Quicksort (z-axis)", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(10, 10, 140, 40), this, 1001);

    mResetButton.Create(L"Reset", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(160, 10, 260, 40), this, 1002);

    mQuicksortY_AXISButton.Create(L"Quicksort (y-axis)", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(10, 60, 140, 40), this, 1003);
    return 0;




}

void CChildView::OnSize(UINT nType, int cx, int cy) {
    COpenGLWnd::OnSize(nType, cx, cy);
    if (mQuicksortButton.GetSafeHwnd())
        mQuicksortButton.MoveWindow(10, 10, 140, 40);

    if (mResetButton.GetSafeHwnd())
        mResetButton.MoveWindow(160, 10, 100, 40);

    if (mQuicksortY_AXISButton.GetSafeHwnd())
        mQuicksortY_AXISButton.MoveWindow(10, 60, 140, 40);
}

void CChildView::OnSortButtonClicked() {
    // Set sorting state
    m_isSorting = true;
    m_useYAxisAnimation = false;

    // Create a thread to save sorting progress across timer invocations
    std::thread worker([this]() {QuickSort(0, (int)mBoxes.size() - 1);m_isSorting = false;});

    // Detach so the thread manages its own lifetime
    worker.detach();
}

void CChildView::OnSortY_AXISButtonClicked() {
    m_isSorting = false;
    m_useYAxisAnimation = true;

    std::thread worker([this]() {QuickSort(0, (int)mBoxes.size() - 1); m_isSorting = false; });
    worker.detach();
}

void CChildView::OnResetOriginalButtonClicked() {
    ResetBoxes();
    Invalidate();
}

void CChildView::OnTimer(UINT_PTR nIDEvent)
{
    float speed = 0.25f;
    float spacing = 3.0f;
    bool anyMoving = false;

    for (int k = 0; k < (int)mBoxes.size(); k++) {
        float destinationX = k * spacing;

        if (abs(mBoxes[k].currentX - destinationX) > 0.05f) {
            anyMoving = true;

            // Move horizontally
            if (mBoxes[k].currentX < destinationX)
                mBoxes[k].currentX += speed;
            else
                mBoxes[k].currentX -= speed;

            float totalDist = abs(destinationX - mBoxes[k].startX);
            if (totalDist > 0.1f) {
                float moved = abs(mBoxes[k].currentX - mBoxes[k].startX);
                float progress = moved / totalDist;
                if (progress > 1.0f) progress = 1.0f;

                if (m_useYAxisAnimation) {
                    // Lift upward, then back down
                    mBoxes[k].offsetY = sin(progress * 3.14159f) * 5.0f;
                    mBoxes[k].offsetZ = 0.0f;
                }
                else {
                    // Existing z-axis arc
                    float direction = (destinationX > mBoxes[k].startX) ? 1.0f : -1.0f;
                    mBoxes[k].offsetZ = sin(progress * 3.14159f) * 5.0f * direction;
                    mBoxes[k].offsetY = 0.0f;
                }
            }
        }
        else {
            // Snap into final position
            mBoxes[k].currentX = destinationX;
            mBoxes[k].startX = destinationX;
            mBoxes[k].offsetZ = 0.0f;
            mBoxes[k].offsetY = 0.0f;
        }
    }

    if (!anyMoving) {
        m_animating = false;
        KillTimer(nIDEvent);
    }

    Invalidate();
}