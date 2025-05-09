using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Sensor : MonoBehaviour
{
    private void OnTriggerEnter(Collider other)
    {
        // OnTriggerEnter : 물리적이지 않은 충돌을 했을 때
        // 호출되는 이벤트 함수입니다.

        if (other.CompareTag("Authorized"))
        {
            Debug.Log("OnTriggerEnter");
        }
    }

    private void OnTriggerStay(Collider other)
    {
        // OnTriggerStay : 물리적이지 않은 충돌을 하고 있을 때
        // 호출되는 이벤트 함수입니다.
        Battery battery = other.GetComponent<Battery>();

        if (battery != null)
        {
            battery.Increase();
        }

        Debug.Log("OnTriggerStay");
    }

    private void OnTriggerExit(Collider other)
    {
        // OnTriggerExit : 물리적이지 않은 충돌이 끝났을 때
        // 호출되는 이벤트 함수입니다.

        Debug.Log("OnTriggerExit");
    }
}
