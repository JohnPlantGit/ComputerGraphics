using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RotationEffect : MonoBehaviour
{
    public Transform m_objectTransform;
    public float m_speed;

    private Vector3 m_previousRoation;

	// Use this for initialization
	void Start ()
    {
        
    }
	
	// Update is called once per frame
	void Update ()
    {
        Vector3 rotation = new Vector3(Random.value, Random.value, Random.value).normalized;
        m_objectTransform.Rotate((m_previousRoation + rotation * Time.deltaTime).normalized, Time.deltaTime * m_speed);
        m_previousRoation = rotation;
    }
}
