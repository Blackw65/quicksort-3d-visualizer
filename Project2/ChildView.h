
// ChildView.h : interface of the CChildView class
//


#pragma once

#include <vector>

#include "graphics/OpenGLWnd.h"
#include "graphics/GrCamera.h"
#include "Mesh.h"
#include <afxwin.h>
#include <atomic>
#include <array>

using namespace std;

struct BoxStruct {
	CMesh mesh;
	int height;
	float startX;
	float currentX;
	float targetX;
	float offsetY;
	float offsetZ;
	std::array<double, 3> color;
};

// CChildView window

class CChildView : public COpenGLWnd
{
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void OnGLDraw(CDC* pDC);

private:
	CGrCamera m_camera;
	CMesh m_mesh;

private:
	void CreateMesh();
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSortButtonClicked();
	afx_msg void OnResetOriginalButtonClicked();
	afx_msg void OnSortY_AXISButtonClicked();


// Here on is project specific stuff

private:

	/*
	* Functions
	*/



	/*
	* Variables
	*/

	// A vector of box objects, each with different height (1-10)
	vector<BoxStruct> mBoxes;
	CButton mQuicksortButton;
	CButton mResetButton;
	CButton mQuicksortY_AXISButton;

	// Save sort progress in between timer activations
	bool m_isSorting = false;
	bool m_useYAxisAnimation = false;
	std::atomic<bool> m_animating;

	// Colors for sorting
	std::array<double, 3> mUNSORTED = { 1.0, 0.0, 0.0 };
	std::array<double, 3> mPIVOT = { 1.0, 1.0, 0.0 };
	std::array<double, 3> mSORTED = { 0.0, 0.0, 1.0 };


	
public:

	/*
	* Functions
	*/

	// Create a singular box
	CMesh CreateBox(double height);

	// Reset boxes to original positions
	// Original position:
	// 5, 7, 3, 1, 4, 6, 2, 10, 8, 9
	void ResetBoxes();

	void QuickSort(int low, int high);
	int Partition(int low, int high);
	void Swap(int x, int y);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

