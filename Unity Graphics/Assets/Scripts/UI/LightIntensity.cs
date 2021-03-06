﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Light))]
public class LightIntensity : MonoBehaviour
{
    private Light m_light;
	// Use this for initialization
	void Start ()
    {
        m_light = GetComponent<Light>();
	}
	
    // Set the intensity of the light to the slider value
    public void SetIntensity(float value)
    {
        m_light.intensity = value;
    }

}
