using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[ExecuteInEditMode]
public class PostProcessing : MonoBehaviour
{
    [Range(0, 1)]
    public float m_intensity;

    private Material m_material;

    private void Awake()
    {
        m_material = new Material(Shader.Find("Hidden/Greyscale"));
    }

    private void OnRenderImage(RenderTexture source, RenderTexture destination)
    {
        if (m_intensity == 0)
        {
            Graphics.Blit(source, destination);
            return;
        }

        m_material.SetFloat("_bwBlend", m_intensity);
        Graphics.Blit(source, destination, m_material);
    }
}
