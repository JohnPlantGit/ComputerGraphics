using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[ExecuteInEditMode]
public class DOFEffect : MonoBehaviour
{
    public CameraController m_camera;
    //[Range(0, 1)]
    //public float m_intensity;
    //[Range(0.1f, 100)]
    //public float m_focusDistance = 10;
    [Range(0.1f, 10)]
    public float m_focusRange = 3;
    [Range(1, 10)]
    public float m_bokehRadius = 4;
    public float m_focusSpeed;

    private Material m_material;
    private const int m_cocPass = 0;
    private const int m_preFilterPass = 1;
    private const int m_bokehPass = 2;
    private const int m_postFilterPass = 3;
    private const int m_combinePass = 4;
    private float m_focusDistance = 0;
    private float m_focusGoal = 0;
	// Use this for initialization
	void Start ()
    {
        m_material = new Material(Shader.Find("Hidden/DepthOfField"));
    }

    private void OnRenderImage(RenderTexture source, RenderTexture destination)
    {
        RaycastHit hit;
        Physics.Raycast(m_camera.DirectionRay, out hit, 9999, LayerMask.NameToLayer("Player"));

        m_focusGoal = hit.distance;

        if (m_focusGoal < m_focusDistance)
        {
            m_focusDistance -= m_focusSpeed * Time.deltaTime;
            if (m_focusDistance < m_focusGoal)
                m_focusDistance = m_focusGoal;
        }
        else
        {
            m_focusDistance += m_focusSpeed * Time.deltaTime;
            if (m_focusDistance > m_focusGoal)
                m_focusDistance = m_focusGoal;
        }

        //m_material.SetFloat("_Intensity", m_intensity);
        m_material.SetFloat("_FocusDistance", m_focusDistance);
        m_material.SetFloat("_FocusRange", m_focusRange);
        m_material.SetFloat("_BokehRadius", m_bokehRadius);

        RenderTexture coc = RenderTexture.GetTemporary(source.width, source.height, 0, RenderTextureFormat.RHalf, RenderTextureReadWrite.Linear);
        RenderTexture dof0 = RenderTexture.GetTemporary(source.width / 2, source.height / 2, 0, source.format);
        RenderTexture dof1 = RenderTexture.GetTemporary(source.width / 2, source.height / 2, 0, source.format);

        m_material.SetTexture("_CoCTex", coc);
        m_material.SetTexture("_DoFTex", dof0);

        Graphics.Blit(source, coc, m_material, m_cocPass);
        Graphics.Blit(source, dof0, m_material, m_preFilterPass);
        Graphics.Blit(dof0, dof1, m_material, m_bokehPass);
        Graphics.Blit(dof1, dof0, m_material, m_postFilterPass);
        Graphics.Blit(source, destination, m_material, m_combinePass);

        RenderTexture.ReleaseTemporary(coc);
        RenderTexture.ReleaseTemporary(dof0);
        RenderTexture.ReleaseTemporary(dof1);
    }
}
