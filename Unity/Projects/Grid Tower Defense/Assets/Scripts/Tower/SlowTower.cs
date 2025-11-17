using System.Collections.Generic;
using NUnit.Framework;
using UnityEngine;

public class SlowTower : BaseTower
{
    [Header("Tower Specific Info")]
    SlowTowerData towerData;

    public List<Enemy> enemiesInRange = new List<Enemy>();

    SphereCollider sphereCollider;

    protected override void Awake()
    {
        base.Awake();

        towerData = (SlowTowerData)base.currentTowerData;

        sphereCollider = GetComponentInChildren<SphereCollider>();
        sphereCollider.radius = towerData.baseRange;
    }

    private void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.CompareTag("Enemy"))
        {
            Enemy inRangeEnemy = other.gameObject.GetComponent<Enemy>();

            enemiesInRange.Add(inRangeEnemy);

            inRangeEnemy.speed *= towerData.slowDownMagnitude;
        }
    }

    private void OnTriggerExit(Collider other)
    {
        if (other.gameObject.CompareTag("Enemy"))
        {
            Enemy inRangeEnemy = other.gameObject.GetComponent<Enemy>();

            enemiesInRange.Remove(inRangeEnemy);

           inRangeEnemy.speed /= towerData.slowDownMagnitude;
        }
    }

    public override void UpgradeTower()
    {
        if (base.currentLevel == 1)
        {
            ResetEnemiesSpeed();

            towerData = (SlowTowerData)levelTwoTowerData;
            sphereCollider.radius = towerData.baseRange;

            UpdateEnemiesSpeed();
        }
        else if (base.currentLevel == 2)
        {
            ResetEnemiesSpeed();

            towerData = (SlowTowerData)maxLevelTowerData;
            sphereCollider.radius = towerData.baseRange;

            UpdateEnemiesSpeed();
        }
        else if (base.currentLevel >= 3)
        {
            return;
        }

        base.UpgradeTower();
    }

    private void ResetEnemiesSpeed()
    {
        foreach (Enemy enemy in enemiesInRange)
        {
            enemy.speed /= towerData.slowDownMagnitude;
        }
    }

    private void UpdateEnemiesSpeed()
    {
        foreach(Enemy enemy in enemiesInRange)
        {
            if (enemy)
            {
                enemy.speed *= towerData.slowDownMagnitude;
            }
        }
    }
}
