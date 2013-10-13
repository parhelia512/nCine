#ifndef CLASS_NCPLOTTINGVARIABLE
#define CLASS_NCPLOTTINGVARIABLE

#include "ncColor.h"
#include "ncProfileVariable.h"
#include "ncDrawableNode.h"
#include "ncRenderCommand.h"
#include "ncRenderQueue.h"

/// A class to group and wrap all the information needed to plot a variable
class ncPlottingVariable
{
protected:
	/// Mean drawing flag
	bool m_bPlotMean;
	/// Graph color
	ncColor m_graphColor;
	/// Mean line color
	ncColor m_meanColor;
	/// The variable to be plotted
	ncProfileVariable m_variable;
	/// The vertices buffer
	float *m_fVertices;

	/// The command used to render variable values
	ncRenderCommand m_valuesCmd;
	/// The command used to render the variable mean
	ncRenderCommand m_meanCmd;
	/// Updates the values rendering command
	virtual void UpdateRenderCommand() = 0;
	/// Updates the mean rendering command
	virtual void UpdateMeanRenderCommand() = 0;

public:
	ncPlottingVariable(unsigned int uNumValues, unsigned int uRejectDelay);
	virtual ~ncPlottingVariable();

	/// Returns the number of value for the variable
	inline unsigned int NumValues() const { return m_variable.NumValues(); }
	/// Returns the normalized mathematic mean of the variable
	inline float NormMean() const { return m_variable.NormMean(); }
	/// Returns the graph color
	inline const ncColor& GraphColor() const { return m_graphColor; }
	/// Returns the mean line color
	inline const ncColor& MeanColor() const { return m_meanColor; }
	/// Returns the constant pointer to the vertices buffer
	inline const float* Vertices() const { return m_fVertices; }
	/// Returns the constant pointer to the vertices buffer
	inline float* Vertices() { return m_fVertices; }
	/// Returns the pointer to the variable
	inline const ncProfileVariable* Variable() const { return &m_variable; }
	/// Returns the state of the mean drawing flag
	inline bool shouldPlotMean() const { return m_bPlotMean; }

	/// Adds a value to the variable
	void AddValue(float fValue) { m_variable.AddValue(fValue); }
	/// Sets the graph color
	void SetGraphColor(ncColor graphColor) { m_graphColor = graphColor; }
	/// Sets the mean line color
	void SetMeanColor(ncColor meanColor) { m_meanColor = meanColor; }
	/// Sets the mean drawing flag state
	void SetPlotMean(bool bEnabled) { m_bPlotMean = bEnabled; }

	// Applies parent transformations to both mean and values vertices
	void ApplyTransformations(float fAbsX, float fAbsY, float fAbsRotation, float fAbsScaleFactor);

	virtual inline void Draw(ncRenderQueue& rRenderQueue)
	{
		UpdateRenderCommand();
		rRenderQueue.AddCommand(&m_valuesCmd);
	}
	virtual inline void DrawMean(ncRenderQueue& rRenderQueue)
	{
		UpdateMeanRenderCommand();
		rRenderQueue.AddCommand(&m_meanCmd);
	}

	virtual void UpdateVertices(int x, int y, int w, int h) = 0;
};

#endif