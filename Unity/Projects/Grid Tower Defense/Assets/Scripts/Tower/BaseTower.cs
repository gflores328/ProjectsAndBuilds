using UnityEngine;
using UnityEngine.EventSystems;

public class BaseTower : MonoBehaviour
{
    [Header("Base Class Info")]
    public BaseTowerData baseTowerData;
    public BaseTowerData levelTwoTowerData;
    public BaseTowerData maxLevelTowerData;
    public BaseTowerData currentTowerData;

    public TowerRangeDrawer towerRangeDrawer;
    public GameManager gameManager;

    public Vector2Int startingCell;

    [Header("Tower Size")]
    public int xSize = 1;
    public int zSize = 1;

    public int currentLevel = 1;

    protected virtual void Awake()
    {
        gameManager = FindFirstObjectByType<GameManager>();

        currentTowerData = baseTowerData;

        towerRangeDrawer.SetTowerData(currentTowerData);
    }
    private void OnMouseDown()
    {

        if (EventSystem.current.IsPointerOverGameObject())
        {
            return;
        }

        if (!gameManager.canSpawnTower)
        {
            return;
        }

        towerRangeDrawer.ShowRange();
        gameManager.TowerClickedOn(this.gameObject);

    }

    public void TowerClickedOff()
    {
        towerRangeDrawer.HideRange();
    }

    public virtual void UpgradeTower()
    {
        if (currentLevel == 1)
        {
            currentTowerData = levelTwoTowerData;
            currentLevel++;
            towerRangeDrawer.SetTowerData(currentTowerData);
        }
        else if (currentLevel == 2)
        {
            currentTowerData = maxLevelTowerData;
            currentLevel++;
            towerRangeDrawer.SetTowerData(currentTowerData);
        }
        else if (currentLevel >= 3)
        {
            return ;
        }
    }
}
