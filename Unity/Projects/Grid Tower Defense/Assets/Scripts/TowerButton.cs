using TMPro;
using Unity.VisualScripting;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class TowerButton : MonoBehaviour, IPointerEnterHandler, IPointerExitHandler
{
    [SerializeField] GameManager gameManager;
    [SerializeField] GhostObject ghostTower;
    [SerializeField] BaseTowerData towerData;

    Button button;
    private void Start()
    {
        button = GetComponent<Button>();
    }
    private void Update()
    {
        if (towerData.cost > gameManager.currentMoney)
        {
            button.interactable = false;
            return;
        }
        button.interactable =  true;
    }
    public void OnButtonClick ()
    {
        gameManager.SpawnTowerFromButton(ghostTower);
    }

    public void OnPointerEnter(PointerEventData eventData)
    {
        gameManager.UpdateDescriptionBox(towerData);
    }

    public void OnPointerExit(PointerEventData eventData)
    {
        gameManager.HideDescriptionBox();
    }
}
