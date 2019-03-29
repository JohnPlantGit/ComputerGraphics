using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[ExecuteInEditMode]
public class CustomSlider : MonoBehaviour
{
    public RectTransform m_fill;
    public RectTransform m_slideArea;
    public RectTransform m_handle;

    public float m_minValue;
    public float m_maxValue;

    public float m_value;

    private bool m_sliding;
	// Use this for initialization
	void Start ()
    {

	}
	
	// Update is called once per frame
	void Update ()
    {
        Vector2 localMousePosition = m_slideArea.InverseTransformPoint(Input.mousePosition);
        if (Input.GetMouseButtonDown(0) && m_slideArea.rect.Contains(localMousePosition))
        {
            m_sliding = true;
        }
        if (m_sliding)
        {
            if (localMousePosition.x < m_slideArea.rect.xMin)
                localMousePosition.x = m_slideArea.rect.xMin;
            if (localMousePosition.x > m_slideArea.rect.xMax)
                localMousePosition.x = m_slideArea.rect.xMax;

            //float newPercentage = (localMousePositionFill.x + Mathf.Abs(m_slideArea.rect.xMin)) / (Mathf.Abs(m_slideArea.rect.xMin) + Mathf.Abs(m_slideArea.rect.xMax));
            //m_value = (newPercentage * (Mathf.Abs(m_slideArea.rect.xMin) + Mathf.Abs(m_slideArea.rect.xMax))) - Mathf.Abs(m_slideArea.rect.xMin);
            float newPercentage = Mathf.Abs(localMousePosition.x - m_slideArea.rect.xMin) / Mathf.Abs(m_slideArea.rect.xMin - m_slideArea.rect.xMax);
            m_value = newPercentage * Mathf.Abs(m_minValue - m_maxValue) + m_minValue;

            if (Input.GetMouseButtonUp(0) && m_sliding)
            {
                m_sliding = false;
            }
        }
        if (Input.GetMouseButtonUp(0) && m_sliding)
        {
            m_sliding = false;
        }

        if (m_value < m_minValue)
            m_value = m_minValue;
        if (m_value > m_maxValue)
            m_value = m_maxValue;

        //float percentage = (m_value + Mathf.Abs(m_minValue)) / (Mathf.Abs(m_minValue) + Mathf.Abs(m_maxValue));
        float percentage = Mathf.Abs(m_value - m_minValue) / Mathf.Abs(m_minValue - m_maxValue);

        m_fill.anchorMax = new Vector2(percentage, 1);
        m_handle.anchorMin = new Vector2(percentage, 0);
        m_handle.anchorMax = new Vector2(percentage, 1);
    }
}
