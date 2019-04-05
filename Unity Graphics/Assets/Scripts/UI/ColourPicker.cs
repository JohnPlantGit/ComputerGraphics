using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ColourPicker : MonoBehaviour
{
    public InputField m_rInput;
    public InputField m_gInput;
    public InputField m_bInput;

    public GameObject m_player;

    private List<Material> m_playerMaterials;

    // Use this for initialization
    void Start ()
    {
        m_playerMaterials = new List<Material>();

        Renderer[] playerRenderers = m_player.GetComponentsInChildren<Renderer>();

        foreach(Renderer current in playerRenderers)
        {
            if (current.material.name.Contains("PlayerMaterial"))
            {
                m_playerMaterials.Add(current.material);
            }
        }

        m_rInput.characterValidation = InputField.CharacterValidation.Integer;
        m_gInput.characterValidation = InputField.CharacterValidation.Integer;
        m_bInput.characterValidation = InputField.CharacterValidation.Integer;

        Color startingColor = m_playerMaterials[0].GetColor("_Outline");

        m_rInput.text = Mathf.RoundToInt((startingColor.r * 255)).ToString();
        m_gInput.text = Mathf.RoundToInt((startingColor.g * 255)).ToString();
        m_bInput.text = Mathf.RoundToInt((startingColor.b * 255)).ToString();
    }

    public void SetColor(string input)
    {
        float r = 0, g = 0, b = 0;
        if (m_rInput.text.Length != 0)
            r = float.Parse(m_rInput.text);
        if (m_gInput.text.Length != 0)
            g = float.Parse(m_gInput.text);
        if (m_bInput.text.Length != 0)
            b = float.Parse(m_bInput.text);

        if (r < 0)
            r = 0;
        if (r > 255)
            r = 255;

        if (g < 0)
            g = 0;
        if (g > 255)
            g = 255;

        if (b < 0)
            b = 0;
        if (b > 255)
            b = 255;

        m_rInput.text = r.ToString();
        m_gInput.text = g.ToString();
        m_bInput.text = b.ToString();

        foreach (Material current in m_playerMaterials)
        {
            current.SetColor("_Outline", new Color(r / 255, g / 255, b / 255));
        }
    }
	
    
}
