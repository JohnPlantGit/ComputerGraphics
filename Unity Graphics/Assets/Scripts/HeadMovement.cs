using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Animator))]
public class HeadMovement : MonoBehaviour
{
    public bool m_limitMovement;
    public bool m_ikActive = false;
    public Transform m_neck;
    public Transform m_root;
    public Transform m_hips;
    public Transform m_chest;
    public CameraController m_camera;
    public float m_lerpLength;
    public ParticleSystem m_leftParticles;
    public ParticleSystem m_rightParticles;

    private Animator m_animator;
    private float m_rightTimer;
    private float m_leftTimer;

	// Use this for initialization
	void Start ()
    {
        m_animator = GetComponent<Animator>();
	}

    private void OnAnimatorIK(int layerIndex)
    {
        Vector3 facing = m_camera.DirectionRay.direction;
        float side = Vector3.SignedAngle(m_root.transform.forward, facing, Vector3.up);
        
        float leftPercentage = m_leftTimer / m_lerpLength;
        float rightPercentage = m_rightTimer / m_lerpLength;


        m_animator.SetIKPosition(AvatarIKGoal.LeftHand, m_neck.position + facing * 1f);
        m_animator.SetIKRotation(AvatarIKGoal.LeftHand, Quaternion.LookRotation(facing));

        m_animator.SetIKPositionWeight(AvatarIKGoal.LeftHand, leftPercentage);
        m_animator.SetIKRotationWeight(AvatarIKGoal.LeftHand, leftPercentage);
       
        m_animator.SetIKPosition(AvatarIKGoal.RightHand, m_neck.position + facing * 1f);
        m_animator.SetIKRotation(AvatarIKGoal.RightHand, Quaternion.LookRotation(facing));
   
        m_animator.SetIKPositionWeight(AvatarIKGoal.RightHand, rightPercentage);
        m_animator.SetIKRotationWeight(AvatarIKGoal.RightHand, rightPercentage);

        m_animator.SetLookAtWeight(1);
        m_animator.SetLookAtPosition(m_chest.position + facing);

        if (m_ikActive)
        {
            if (side < -45)
            {
                m_leftTimer += Time.deltaTime;
                if (m_leftTimer > m_lerpLength)
                    m_leftTimer = m_lerpLength;

                m_rightTimer -= Time.deltaTime;
                if (m_rightTimer < 0)
                    m_rightTimer = 0;

                m_leftParticles.Play();
                m_rightParticles.Stop();
            }
            else if (side > -45 && side < 45)
            {
                m_leftTimer += Time.deltaTime;
                if (m_leftTimer > m_lerpLength)
                    m_leftTimer = m_lerpLength;

                m_rightTimer += Time.deltaTime;
                if (m_rightTimer > m_lerpLength)
                    m_rightTimer = m_lerpLength;

                m_leftParticles.Play();
                m_rightParticles.Play();
            }
            else if (side > 45)
            {
                m_rightTimer += Time.deltaTime;
                if (m_rightTimer > m_lerpLength)
                    m_rightTimer = m_lerpLength;

                m_leftTimer -= Time.deltaTime;
                if (m_leftTimer < 0)
                    m_leftTimer = 0;

                m_leftParticles.Stop();
                m_rightParticles.Play();
            }
        }
        else
        {
            m_leftTimer -= Time.deltaTime;
            if (m_leftTimer < 0)
                m_leftTimer = 0;
            m_rightTimer -= Time.deltaTime;
            if (m_rightTimer < 0)
                m_rightTimer = 0;            

            m_leftParticles.Stop();
            m_rightParticles.Stop();
        }
    }
}
