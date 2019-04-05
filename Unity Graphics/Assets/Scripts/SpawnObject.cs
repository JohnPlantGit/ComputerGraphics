using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SpawnObject : MonoBehaviour
{
    public ObjectList m_objectList;
    public CameraController m_camera;
    public PrimitiveType[] m_objects;
    public RectTransform[] m_transforms;

    // Use this for initialization
    void Start ()
    {
		
	}
	
	// Update is called once per frame
	void Update ()
    {
        if (Input.GetMouseButtonDown(0))
        {
            for (int i = 0; i < m_transforms.Length; i++)
            {
                Vector2 localMousePosition = m_transforms[i].InverseTransformPoint(Input.mousePosition);
                if (m_transforms[i].rect.Contains(localMousePosition))
                {
                    RaycastHit hit;
                    if (Physics.Raycast(m_camera.DirectionRay, out hit, 9999, LayerMask.NameToLayer("Player")))
                    {
                        GameObject newObject = GameObject.CreatePrimitive(m_objects[i]);
                        Mesh mesh = newObject.GetComponent<MeshFilter>().mesh;
                        float yMin = mesh.vertices[0].y;
                        foreach (Vector3 current in mesh.vertices)
                        {
                            if (current.y < yMin)
                                yMin = current.y;
                        }
                        newObject.transform.position = hit.point - new Vector3(0, yMin, 0);
                        newObject.transform.rotation = Quaternion.Euler(0, 0, 0);
                        m_objectList.AddElement(newObject);                        
                    }

                }
            }
        }
    }
}
