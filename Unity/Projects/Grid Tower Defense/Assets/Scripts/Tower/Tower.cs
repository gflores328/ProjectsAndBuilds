using UnityEngine;
using System.Collections.Generic;
using System.Collections;
using Unity.VisualScripting;


public class Tower : BaseTower
{
    
    TowerData towerData;

    [Header("Tower Specific Info")]
    public GameObject firePoint;
    public GameObject projectile;
    public bool shouldRotate;
    public GameObject rotationPoint;

    SphereCollider rangeCollider;

    List<GameObject> inRangeEnemies = new List<GameObject>();

    bool canFire = true;

    public float damageMultipler = 1;

    SphereCollider sphereCollider;

    public List<BuffTower> buffTowers = new List<BuffTower>();

    protected override void Awake()
    {
        base.Awake();

        towerData = (TowerData)base.baseTowerData;

        sphereCollider = GetComponentInChildren<SphereCollider>();
        sphereCollider.radius = towerData.baseRange;
    }

    private void Update()
    {
        if (shouldRotate && inRangeEnemies.Count > 0)
        {
            if (inRangeEnemies[0] != null)
            {
                rotationPoint.transform.LookAt(inRangeEnemies[0].gameObject.transform.position + new Vector3(0,.3f,0));
            }
        }

        if (inRangeEnemies.Count > 0 && canFire)
        {
            Fire(inRangeEnemies[0]);
        }
    }

    private void OnTriggerEnter(Collider other)
    {
        if (other.CompareTag("Enemy"))
        {
            inRangeEnemies.Add(other.gameObject);
        }
    }

    private void OnTriggerExit(Collider other)
    {
        if (other.CompareTag("Enemy"))
        {
            inRangeEnemies.Remove(other.gameObject);
        }
    }

    private void Fire(GameObject enemyToFireAt)
    {
        if (enemyToFireAt == null)
        {
            inRangeEnemies.Remove(enemyToFireAt);
            return;
        }
        
        Vector3 direction = (enemyToFireAt.transform.position - firePoint.transform.position).normalized + new Vector3(0, .3f, 0);

        GameObject firedProjectile = Instantiate(projectile, firePoint.transform.position, Quaternion.LookRotation(direction));
        firedProjectile.GetComponent<Projectile>().damage = towerData.damage * damageMultipler;
        firedProjectile.GetComponent<Rigidbody>().linearVelocity = direction * towerData.projectileSpeed; 

        canFire = false;
        StartCoroutine(ProjectileCooldown());
    }

    private IEnumerator ProjectileCooldown()
    {
        yield return new WaitForSeconds(towerData.fireCoolDown);
        canFire = true;
    }

    public override void UpgradeTower()
    {
        if (base.currentLevel == 1)
        {
            towerData = (TowerData)levelTwoTowerData;
            sphereCollider.radius = towerData.baseRange;
        }
        else if (base.currentLevel == 2)
        {
            towerData = (TowerData)maxLevelTowerData;
            sphereCollider.radius = towerData.baseRange;   
        }
        else if (base.currentLevel >= 3)
        {
            return;
        }

        base.UpgradeTower();
    }

    

    private void OnDestroy()
    {
        foreach (BuffTower buffTower in buffTowers)
        {
            buffTower.RemoveFromList(this);
        }
    }

}
