using System.Collections;
using System.Collections.Generic;
using Unity.VisualScripting;
using UnityEngine;

public class Automation : MonoBehaviour
{
    [SerializeField] bool state;
    [SerializeField] Vector3 direction;
    [SerializeField] Rigidbody rigidbody;

    private void Awake()
    {
        rigidbody = GetComponent<Rigidbody>();
    }

    private void Start()
    {
        StartCoroutine(Coroutine());
    }

    private void FixedUpdate()
    {
        // ForceMode.Force : 매 프레임마다 지속적인 힘을 가하는 함수입니다. (Mass) - O

        // ForceMode.Impulse : 순간적인 힘을 가하는 함수입니다. (Mass) - O

        // ForceMode.Acceleration : 지속적인 가속도를 적용하는 함수입니다. (Mass) - X

        // ForceMode.VelocityChange : 순간적인 속도 변경을 적용하는 함수입니다. (Mass) - X

        if (state)
        {
            rigidbody.AddForce(direction, ForceMode.Impulse);

            state = false;
        }
    }

    IEnumerator Coroutine()
    {
        while (true)
        {
            yield return new WaitForSeconds(5);

            state = true;
        }
    }
}
