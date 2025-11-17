using UnityEngine;
using UnityEngine.InputSystem;

public class CameraDrag : MonoBehaviour
{
    private Vector2 delta;

    private bool isMoving;

    [SerializeField] float dragSpeed = 10;
    public void OnLook(InputAction.CallbackContext context)
    {
        delta = context.ReadValue<Vector2>();
    }

    public void OnMove(InputAction.CallbackContext context)
    {
        isMoving = context.started || context.performed;
    }

    private void LateUpdate()
    {
        if (isMoving)
        {
            var position = -transform.right * (delta.x * dragSpeed);
            position += transform.up * (delta.y * -dragSpeed);
            transform.position += position * Time.deltaTime;
        }
    }
}
