using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ColourPicker : MonoBehaviour
{
    public InputField m_rInput;
    public InputField m_gInput;
    public InputField m_bInput;

    public Material m_playerMaterial;

    // Use this for initialization
    void Start ()
    {
        m_rInput.characterValidation = InputField.CharacterValidation.Decimal;
        m_gInput.characterValidation = InputField.CharacterValidation.Decimal;
        m_bInput.characterValidation = InputField.CharacterValidation.Decimal;              
    }

    public void SetColor(string input)
    {
        float r, g, b;
        r = float.Parse(m_rInput.text);
        g = float.Parse(m_gInput.text);
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


        m_playerMaterial.SetColor("Outline color", new Color(r, g, b));
    }
	
    
}
