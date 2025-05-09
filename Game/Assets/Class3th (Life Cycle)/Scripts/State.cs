using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class State : MonoBehaviour
{
    private void Awake()
    {
        // Awake 함수란?
        // 게임 오브젝트가 생성되었을 때 단 한 번만 호출되며,
        // 스크립트가 비활성화되어 있는 상태에서도 호출되는 이벤트 함수입니다.

        Debug.Log("Awake");
    }

    private void OnEnable()
    {
        // OnEnable 함수란?
        // 게임 오브젝트가 활성화되는 순간마다 호출되는 이벤트 함수입니다.

        Debug.Log("OnEnable");
    }

    private void Start()
    {
        // Start 함수란?
        // 게임 오브젝트가 생성되었을 때 단 한 번만 호출되며,
        // 스크립트가 비활성화된 상태일 땐, 호출되지 않는 이벤트 함수입니다.

        Debug.Log("Start");
    }

    private void FixedUpdate()
    {
        // FixedUpdate 함수란?
        // TimeStep에 설정된 값에 따라 일정한 간격으로 호출되는 이벤트 함수입니다.

        Debug.Log("Fixed Update");
    }

    private void Update()
    {
        // Update 함수란?
        // 게임이 실행되는 동안 매 프레임마다 호출되는 이벤트 함수입니다.

        Debug.Log("Update");
    }

    private void LateUpdate()
    {
        // Late Update
        // Update 함수가 호출된 이후에 호출되는 이벤트 함수입니다.

        Debug.Log("Late Update");
    }

    private void OnDisable()
    {
        // OnDisable
        // 게임 오브젝트가 비활성화 되었을 때 호출되는 이벤트 함수입니다.

        Debug.Log("OnDisable");
    }

    private void OnDestroy()
    {
        // OnDestroy
        // 게임 오브젝트가 삭제되었을 때 호출되는 이벤트 함수입니다.

        Debug.Log("OnDestroy");
    }
}
