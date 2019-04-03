using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RemoveObject : MonoBehaviour
{
    public GameObject m_object;
    public ObjectList m_objectList;

    public void Remove()
    {
        Destroy(m_object);
        m_objectList.RemoveElement(gameObject);
    }
}
