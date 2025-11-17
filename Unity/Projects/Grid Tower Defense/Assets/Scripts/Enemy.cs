using Unity.Mathematics;
using UnityEngine;
using UnityEngine.Splines;

public class Enemy : MonoBehaviour
{
    public SplineContainer spline;

    public float speed;
    public float health;
    public int moneyPerHit;
    public int damage;

    float lengthOnSpline;

    public GameManager gameManager;
    public SpawnManager spawnManager;

    private void Start()
    {
        
    }
    void Update()
    {
        lengthOnSpline += (speed / spline.CalculateLength()) * Time.deltaTime;

        if (lengthOnSpline > 1 )
        {
            DamagePlayer(damage);

            spawnManager.RemoveSelfFromList(this);

            Destroy(gameObject);
        }

        spline.Evaluate(lengthOnSpline, out float3 position, out float3 tangent, out float3 up);

        transform.position = new Vector3(position.x, 0, position.z);
        transform.rotation = Quaternion.LookRotation(tangent, up);
    }

    public void TakeDamage(float damageToTake)
    {
        health -= damageToTake;
        gameManager.UpdateMoney(moneyPerHit);
        if (health <= 0)
        {
            spawnManager.RemoveSelfFromList(this);
            Destroy(gameObject);
        }
    }

    private void DamagePlayer(int amount)
    {
        gameManager.UpdateHealth(-amount);
    }
}
