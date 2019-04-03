using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class ObjectList : MonoBehaviour
{
    public GameObject[] m_objects;
    public LinkedList<GameObject> m_UIElements;// = new LinkedList<GameObject>();
    public GameObject m_template;
    public RectTransform m_content;

    private Vector2 m_elementSize;
	// Use this for initialization
	void Start ()
    {
        m_UIElements = new LinkedList<GameObject>();

        m_elementSize.y = m_template.GetComponent<RectTransform>().rect.height;
        m_elementSize.x = m_template.GetComponent<RectTransform>().rect.width;

        m_content.SetSizeWithCurrentAnchors(RectTransform.Axis.Vertical, m_elementSize.y * m_objects.Length);

        foreach (GameObject current in m_objects)
        {
            GameObject newElement = Instantiate(m_template, m_content);
            newElement.GetComponent<Text>().text = current.name;
            newElement.GetComponent<RemoveObject>().m_object = current;
            newElement.GetComponent<RemoveObject>().m_objectList = this;

            if (m_UIElements.Count == 0)
            {
                newElement.GetComponent<RectTransform>().localPosition = new Vector3(m_elementSize.x / 2, m_content.rect.yMax, 0);
            }
            else
            {
                newElement.GetComponent<RectTransform>().localPosition = new Vector3(m_elementSize.x / 2, -(m_elementSize.y * m_UIElements.Count), 0);
            }

            m_UIElements.AddLast(newElement);
        }
    }
	
	public void RemoveElement(GameObject toRemove)
    {
        LinkedListNode<GameObject> node = m_UIElements.Find(toRemove);

        Destroy(node.Value);
        m_UIElements.Remove(node);

        m_content.SetSizeWithCurrentAnchors(RectTransform.Axis.Vertical, m_elementSize.y * m_UIElements.Count);
        //while (current != null)
        //{
        //    current.Value.GetComponent<RectTransform>().localPosition += new Vector3(0, (m_elementSize.y / 2), 0);

        //    current = current.Next;
        //}

        int index = 0;
        foreach(GameObject current in m_UIElements)
        {
            current.GetComponent<RectTransform>().localPosition = new Vector3(m_elementSize.x / 2, -(m_elementSize.y * index), 0);
            index++;
        }
    }
}
