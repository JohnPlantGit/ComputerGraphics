using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[ExecuteInEditMode]
[RequireComponent(typeof(Terrain))]
public class TerrainGenerator : MonoBehaviour
{
    public bool m_generateTerrain = false;
    public int m_octaves = 20;
    public float m_amplitude = 0.5f;
    public float m_persistence = 0.3f;

    private Terrain m_terrain;
	// Use this for initialization
	void Start ()
    {
        m_terrain = GetComponent<Terrain>();

        //GenerateTerrain();
	}
	
	// Update is called once per frame
	void Update ()
    {
		if (m_generateTerrain)
        {
            GenerateTerrain();

            m_generateTerrain = false;
        }
	}
    void GenerateTerrain()
    {
        int resolution = m_terrain.terrainData.heightmapResolution;
        float scale = (1.0f / resolution) * 3;

        float[,] heightMap = new float[resolution, resolution];

        float startPos = Random.Range(0, 5000);

        for (int x = 0; x < resolution; x++)
        {
            for (int y = 0; y < resolution; y++)
            {
                float height = 0;
                float amplitude = m_amplitude;

                for (int octave = 0; octave < m_octaves; octave++)
                {
                    float freq = Mathf.Pow(2, octave);

                    float sample = Mathf.PerlinNoise(startPos + x * scale * freq, startPos + y * scale * freq);

                    height += sample * amplitude;

                    amplitude *= m_persistence;
                }

                heightMap[x, y] = height;
            }
        }

        m_terrain.terrainData.SetHeights(0, 0, heightMap);

        //m_terrain.UpdateGIMaterials();
    }
}

