using UnityEngine;
using UnityEngine.UIElements;

public class BaseTowerData : ScriptableObject
{
    [Header("Base Data")]
    public string towerName;
    public int cost;
    public int sellPrice;
    public float baseRange;
    public int upgradeCost;

    [TextArea]
    public string description;
    
}
