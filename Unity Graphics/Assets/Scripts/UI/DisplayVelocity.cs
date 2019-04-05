using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class DisplayVelocity : MonoBehaviour
{
    public CustomCharacterController m_character;
    private Text m_text;
	// Use this for initialization
	void Start ()
    {
        m_text = GetComponent<Text>();
	}
	
	// Update is called once per frame
	void Update ()
    {
        m_text.text = m_character.Velocity.ToString() + " Mag: " + m_character.Velocity.magnitude.ToString("0.##");
	}
}
