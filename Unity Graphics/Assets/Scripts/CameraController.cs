using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraController : MonoBehaviour
{
    public GameObject m_parent;
    public GameObject m_camera;
    public float m_armLength;
    public float m_pitchUpperLimit;
    public float m_pitchLowerLimit;

    private float m_yaw = 0.0f;
    private float m_pitch = 0.0f;
    private Vector3 m_offset;
    private Vector3 m_cameraDirection;

    public float Yaw
    {
        get { return m_yaw; }
    }
    public float Pitch
    {
        get { return m_pitch; }
    }

    // Creates a ray coming out of the camera
    public Ray DirectionRay
    {
        get
        {
            Ray output = new Ray();
            output.origin = m_camera.transform.position;
            output.direction = m_camera.transform.forward;
            return output;
        }
    }

    // Use this for initialization
    void Start ()
    {
        m_offset = transform.position - m_parent.transform.position;
        m_cameraDirection = (m_camera.transform.position - transform.position).normalized;
        Cursor.lockState = CursorLockMode.Locked;
	}
	
	// Update is called once per frame
	void LateUpdate ()
    {
        //float cameraSpeed = 50;
        //cameraSpeed *= Time.deltaTime;
        //if ((transform.position - (m_parent.transform.position + m_offset)).magnitude > 0.01)
        //transform.position = Vector3.Lerp(transform.position, m_parent.transform.position + m_offset, cameraSpeed);
        transform.position = m_parent.transform.position + m_offset; // moves the camera with the parent

        // lock and unlock the mouse
        if (Input.GetKeyDown(KeyCode.Escape))
        {
            if (Cursor.lockState != CursorLockMode.Locked)
            {
                Cursor.lockState = CursorLockMode.Locked;
                Cursor.visible = false;                
            }
            else
            {
                Cursor.lockState = CursorLockMode.None;
                Cursor.visible = true;
            }
        }

        // if the mouse if locked
        if (Cursor.lockState == CursorLockMode.Locked)
        {
            m_yaw += Input.GetAxis("Mouse X") * 2;
            if (m_yaw > 180)
            {
                m_yaw -= 360;
            }
            if (m_yaw < -180)
            {
                m_yaw += 360;
            }
            m_pitch -= Input.GetAxis("Mouse Y") * 2;
            if (m_pitch > m_pitchUpperLimit)
            {
                m_pitch = m_pitchUpperLimit;
            }
            if (m_pitch < m_pitchLowerLimit)
            {
                m_pitch = m_pitchLowerLimit;
            }
        }

        // update the rotation of the camera
        transform.eulerAngles = new Vector3(m_pitch, m_yaw, 0);
        
        RaycastHit raycast;
        Vector3 direction = Quaternion.Euler(m_pitch, m_yaw, 0) * m_cameraDirection;
        
        // checks for objects hitting the camera so it will move inwards when you reach a wall
        if (Physics.Raycast(transform.position, direction, out raycast, Mathf.Abs(m_armLength), LayerMask.NameToLayer("Player")))
        {
            Vector3 pos = m_camera.transform.localPosition;
            pos.z = -raycast.distance;
            m_camera.transform.localPosition = pos;
        }
        else
        {
            Vector3 pos = m_camera.transform.localPosition;
            pos.z = m_armLength;
            m_camera.transform.localPosition = pos;
        }

    }
}
