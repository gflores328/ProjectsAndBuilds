using UnityEngine;
using System.Collections.Generic;

public class BuffTower : BaseTower
{
    [Header("Tower Specific Info")]
    BuffTowerData towerData;

    List<Tower> inRangeTowers = new List<Tower>();

    SphereCollider sphereCollider;
    protected override void Awake()
    {
        base.Awake();

        towerData = (BuffTowerData)base.baseTowerData;

        sphereCollider = GetComponentInChildren<SphereCollider>();
        sphereCollider.radius = towerData.baseRange;
    }

    private void OnTriggerEnter(Collider other)
    {

        if (other.gameObject.CompareTag("Tower") && !inRangeTowers.Contains(other.gameObject.GetComponent<Tower>()))
        {
            Tower inRangeTower = other.gameObject.GetComponent<Tower>();
            inRangeTower.damageMultipler *= towerData.buffMagnitude;
           
            inRangeTowers.Add(inRangeTower);

            inRangeTower.buffTowers.Add(this);
        }
    }

    private void OnTriggerExit(Collider other)
    {
        if (other.gameObject.CompareTag("Tower"))
        {
            Tower inRangeTower = other.gameObject.GetComponent<Tower>();
            inRangeTowers.Remove(inRangeTower);
        }
    }

    public override void UpgradeTower()
    {
        if (base.currentLevel == 1)
        {
            ResetBuffEffect();

            towerData = (BuffTowerData)levelTwoTowerData;
            sphereCollider.radius = towerData.baseRange;

            UpdateBuffEffect();

        }
        else if (base.currentLevel == 2)
        {
            ResetBuffEffect();

            towerData = (BuffTowerData)maxLevelTowerData;
            sphereCollider.radius = towerData.baseRange;

            UpdateBuffEffect();

        }
        else if (base.currentLevel >= 3)
        {
            return;
        }

        base.UpgradeTower();
    }

    private void ResetBuffEffect()
    {
        foreach (Tower tower in inRangeTowers)
        {
            tower.damageMultipler /= towerData.buffMagnitude;
        }
    }

    private void UpdateBuffEffect()
    {
        foreach (Tower tower in inRangeTowers)
        {
            tower.damageMultipler *= towerData.buffMagnitude;
        }
    }

    public void RemoveFromList(Tower towerToRemove)
    {
        inRangeTowers.Remove(towerToRemove);
    }
}
