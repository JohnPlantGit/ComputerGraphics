using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CustomCharacterController : MonoBehaviour
{
    public float m_acceleration;
    public float m_crouchedAcceleration;
    public float m_friction;
    public float m_jumpSpeed;
    public float m_crouchLength;
    public CameraController m_cameraController = null;

    private Animator m_animator = null;
    private CapsuleCollider m_collider = null;
    private HeadMovement m_ik;
    private Vector3 m_velocity;
    private bool m_grounded = false;
    private bool m_crouching = false;
    private float m_colliderHeight;
    private float m_colliderCentre;
    private float m_crouchTimer;

    public Vector3 Velocity
    {
        get { return m_velocity; }
    }


	// Use this for initialization
	void Start ()
    {
        m_animator = GetComponent<Animator>();
        m_collider = GetComponent<CapsuleCollider>();
        m_ik = GetComponent<HeadMovement>();

        m_colliderHeight = m_collider.height;
        m_colliderCentre = m_collider.center.y;

        //Cursor.lockState = CursorLockMode.Locked;

        m_crouchTimer = m_crouchLength;
	}
	
	// Update is called once per frame
	void Update ()
    {
        // Jump
        if (m_grounded && Input.GetKeyDown(KeyCode.Space))
        {
            m_velocity += new Vector3(0, m_jumpSpeed, 0);
            m_animator.SetTrigger("Jump");
        }

        // Run the punch animation
        if (Input.GetMouseButtonDown(0))
        {
            m_animator.SetTrigger("Punch");
        }

        // Turn on inverse kinematics
        if (Input.GetMouseButtonDown(1))
        {
            m_ik.m_ikActive = true;
        }
        // turn off inverse kinematics
        if (Input.GetMouseButtonUp(1))
        {
            m_ik.m_ikActive = false;
        }
        //m_velocity += Physics.gravity * Time.deltaTime * m_friction * 0.5f;

        // Courch
        if (Input.GetKeyDown(KeyCode.LeftControl))
        {
            // Stops the character from standing when there is something above them
            if (m_crouching)
            {
                RaycastHit[] raycast = Physics.RaycastAll(transform.position, Vector3.up, 2.0f);
                if ((raycast.Length > 0 && raycast[0].distance >= 2) || raycast.Length == 0)
                {
                    m_crouching = false;
                }
            }
            else
            {
                m_crouching = true;
            }    
        }

        // crouch timer
        if (m_crouching)
            m_crouchTimer -= Time.deltaTime;
        else
            m_crouchTimer += Time.deltaTime;
        if (m_crouchTimer > m_crouchLength)
            m_crouchTimer = m_crouchLength;
        if (m_crouchTimer < 0)
            m_crouchTimer = 0;

        // used to blend between the standing and crouching animations
        m_animator.SetLayerWeight(0, m_crouchTimer / m_crouchLength);
        m_animator.SetLayerWeight(1, 1 - (m_crouchTimer / m_crouchLength));

        // lerps the size of the collider when crouching
        m_collider.height = Mathf.Lerp(m_colliderHeight / 2, m_colliderHeight, m_crouchTimer / m_crouchLength);
        m_collider.center = new Vector3(0, Mathf.Lerp(m_colliderCentre / 2, m_colliderCentre, m_crouchTimer / m_crouchLength), 0);

        // gets the input in the direction the camera is facing
        Vector3 movementVector = Quaternion.Euler(0, m_cameraController.Yaw, 0) * (new Vector3(Input.GetAxis("Horizontal"), 0, Input.GetAxis("Vertical"))).normalized;

        m_velocity += movementVector * (m_crouching ? m_crouchedAcceleration : m_acceleration) * Time.deltaTime;
        transform.position += m_velocity * Time.deltaTime;
        m_velocity -= m_velocity * m_friction * Time.deltaTime;

        Vector3 lookVector = new Vector3(m_velocity.x, 0, m_velocity.z); // looks in the direction of movement
        if (lookVector.sqrMagnitude >= 0.001) // update to only use velocity x / z
        {
            transform.rotation = Quaternion.LookRotation(lookVector.normalized);
        }
        m_animator.SetFloat("Speed", new Vector3(m_velocity.x, 0, m_velocity.z).magnitude);

        if (movementVector.magnitude == 0 && m_grounded && m_velocity.magnitude <= 0.1)
        {
            m_velocity = Vector3.zero;
        }

        if (!m_grounded)
            m_velocity += Physics.gravity * Time.deltaTime * m_friction * 0.5f;
    }

    // Handles collisions
    private void LateUpdate()
    {
        Collider[] collisions = Physics.OverlapCapsule(m_collider.center + new Vector3(0, m_collider.height / 2, 0) + transform.position, m_collider.center - new Vector3(0, m_collider.height / 2, 0) + transform.position, m_collider.radius, -1, QueryTriggerInteraction.Ignore);

        foreach (Collider c in collisions)
        {
            if (c == m_collider)
                continue;

            Vector3 direction;
            float distance;
            if (Physics.ComputePenetration(m_collider, transform.position, Quaternion.Euler(0, 0, 0), c, c.transform.position, c.transform.rotation, out direction, out distance))
            {
                Vector3 penetration = direction * distance;
                Vector3 velocityProjected = Vector3.Project(m_velocity, -direction);
              
                transform.position += penetration;
                m_velocity -= velocityProjected;               
            }
        }

        Vector3 origin = transform.position;
        origin.y += 0.1f;
        if (Physics.Raycast(origin, Vector3.down, 0.2f))
        {
            m_grounded = true;
        }
        else
        {
            m_grounded = false;
        }
    }
}
