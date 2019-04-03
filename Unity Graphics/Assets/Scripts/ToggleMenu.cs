using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ToggleMenu : MonoBehaviour
{
    public GameObject[] m_children;
    public bool m_startActive;

    private bool m_active = true;
    private Text m_text;

    private void Start()
    {
        m_active = m_startActive;

        m_text = GetComponentInChildren<Text>();

        foreach (GameObject child in m_children)
        {
            child.SetActive(m_active);
        }

        if (m_active)
            m_text.text = "Hide Menu";
        else
            m_text.text = "Show Menu";
    }

    public void Toggle()
    {
        m_active = !m_active;

        foreach(GameObject child in m_children)
        {
            child.SetActive(m_active);
        }

        if (m_active)
            m_text.text = "Hide Menu";
        else
            m_text.text = "Show Menu";
    }	
}
