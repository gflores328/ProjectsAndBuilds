using TMPro;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class GameManager : MonoBehaviour
{

    public bool canSpawnTower = true;
    //public bool canPlaceTower = false;

    [SerializeField] GridManager gridManager;
    [SerializeField] SpawnManager spawnManager;

    [SerializeField] GameObject descriptionUI;

    public int currentMoney = 500;
    public int currentHealth = 100;
    [SerializeField] TextMeshProUGUI moneyText;
    [SerializeField] TextMeshProUGUI roundText;
    [SerializeField] TextMeshProUGUI healthText;

    [Header("UI")]
    [SerializeField] GameObject buyTowerUI;
    [SerializeField] GameObject selectedTowerUI;

    GameObject clickedOnTower;

    public GameObject WinUI;
    public GameObject LoseUI;

    public GameObject PauseMenu;
    GameObject spawnedPauseMenu;
    bool isPaused = false;

    public TowerSelectedUI towerSelectedData;

    private void Start()
    {
        moneyText.text = "$: " + currentMoney;
        healthText.text = "Health: " + currentHealth;
    }

    public void SpawnTowerFromButton(GhostObject towerToSpawn)
    {
        if (canSpawnTower)
        {
            GameObject spawnedTower = Instantiate(towerToSpawn.gameObject);
            canSpawnTower = false;
            // canPlaceTower = true;
        }
    }

    public void PlaceTower(BaseTower towerToPlace, Vector3 towerPosition, Vector3 startingCell)
    {

        GameObject placedTower = Instantiate(towerToPlace.gameObject, towerPosition, Quaternion.identity);
        placedTower.GetComponent<BaseTower>().startingCell = new Vector2Int(Mathf.RoundToInt(startingCell.x), Mathf.RoundToInt(startingCell.z));

        placedTower.GetComponent<BaseTower>().gameManager = this;

        UpdateMoney(-placedTower.GetComponent<BaseTower>().currentTowerData.cost);

        canSpawnTower = true;

        gridManager.RemoveCellsFromData(new Vector2Int(Mathf.RoundToInt(startingCell.x), Mathf.RoundToInt(startingCell.z)), placedTower.GetComponent<BaseTower>());
    }

    public void UpdateMoney(int amount)
    {
        currentMoney += amount;
        moneyText.text = "$: " + currentMoney;
    }

    public void UpdateDescriptionBox(BaseTowerData towerData)
    {
        descriptionUI.SetActive(true);
        descriptionUI.GetComponentInChildren<TextMeshProUGUI>().text = towerData.description;
    }

    public void HideDescriptionBox()
    {
        descriptionUI.SetActive(false);
    }

    public void UpdateRoundNumber(int roundNumber)
    {
        roundText.text = "Round: " + roundNumber;
    }

    public void UpdateHealth(int amount)
    {
        currentHealth += amount;
        healthText.text = "Health: " + currentHealth;

        if (currentHealth <= 0)
        {
            GameLost();
        }
    }

    public void TowerClickedOn(GameObject clickedTower)
    {
        if (canSpawnTower)
        {
            if (clickedOnTower != clickedTower && clickedOnTower != null)
            {
                clickedOnTower.GetComponent<BaseTower>().TowerClickedOff();
            }
            clickedOnTower = clickedTower;

            UpdateSelectedTowerUI();

            buyTowerUI.SetActive(false);
            selectedTowerUI.SetActive(true);
        }
    }

    public void SellTower()
    {
        BaseTower towerToSell = clickedOnTower.GetComponent<BaseTower>();

        UpdateMoney(towerToSell.currentTowerData.sellPrice);
        gridManager.AddCellsToData(new Vector2Int(towerToSell.startingCell.x, towerToSell.startingCell.y), towerToSell);

        //gridManager.AddCellToData(new Vector2Int(Mathf.RoundToInt(clickedOnTower.transform.position.x), Mathf.RoundToInt(clickedOnTower.transform.position.z)));

        buyTowerUI.SetActive(true);
        selectedTowerUI.SetActive(false);

        Destroy(clickedOnTower);
        clickedOnTower = null;
    }

    public void UpgradeTower()
    {
        BaseTower currentClickedTower = clickedOnTower.GetComponent<BaseTower>();
        BaseTowerData clickedOnTowerData = currentClickedTower.currentTowerData;

        if (currentClickedTower.currentLevel < 3 && currentMoney >= clickedOnTowerData.upgradeCost)
        {
            UpdateMoney(-clickedOnTowerData.upgradeCost);

            currentClickedTower.UpgradeTower();
            UpdateSelectedTowerUI();
        }
    }

    private void Update()
    {
        if (Input.GetMouseButtonDown(0) && canSpawnTower)
        {
            Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);
            if (Physics.Raycast(ray, out RaycastHit hit) && !EventSystem.current.IsPointerOverGameObject())
            {
                if (hit.collider.gameObject != clickedOnTower && clickedOnTower != null)
                {
                    clickedOnTower.GetComponent<BaseTower>().TowerClickedOff();
                    clickedOnTower = null;

                    buyTowerUI.SetActive(true);
                    selectedTowerUI.SetActive(false);
                }
            }
        }
    }

    public void GameWon()
    {
        Time.timeScale = 0;
        Instantiate(WinUI);
    }

    public void GameLost()
    {
        Time.timeScale = 0;
        Instantiate(LoseUI);
    }

    public void TogglePause()
    {
        if (!isPaused)
        {
            GamePause();
            isPaused = true;
            return;
        }

        GameUnpause();
        isPaused = false;
    }

    private void GamePause()
    {
        Time.timeScale = 0;
        spawnedPauseMenu = Instantiate(PauseMenu);

        spawnedPauseMenu.GetComponentInChildren<Button>().onClick.AddListener(TogglePause);
    }

    private void GameUnpause()
    {
        Time.timeScale = 1;
        Destroy(spawnedPauseMenu);
    }

    private void UpdateSelectedTowerUI()
    {
        BaseTowerData clickedOnTowerData = clickedOnTower.GetComponent<BaseTower>().currentTowerData;

        towerSelectedData.towerNameText.text = clickedOnTowerData.towerName;
        if (clickedOnTower.GetComponent<BaseTower>().currentLevel < 3)
        {
            towerSelectedData.towerUpgradePriceText.text = "$" + clickedOnTowerData.upgradeCost;
        }
        else
        {
            towerSelectedData.towerUpgradePriceText.text = "Max Level";
        }
    }
}
