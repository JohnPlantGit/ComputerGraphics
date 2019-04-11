using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RemoveObject : MonoBehaviour
{
    public GameObject m_object;
    public ObjectList m_objectList;

    // destorys the object and removes it from the object list
    public void Remove()
    {
        Destroy(m_object);
        m_objectList.RemoveElement(gameObject);
    }
}
