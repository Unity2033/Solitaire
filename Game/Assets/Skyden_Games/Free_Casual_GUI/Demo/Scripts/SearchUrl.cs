using UnityEngine;
using UnityEngine.UI;

public class SearchUrl : MonoBehaviour
{
    [Header("Settings")]
    [SerializeField] private string url; // Default URL
    [SerializeField] private Button searchButton; // Assign in Inspector

    private void Start()
    {
        if (searchButton != null)
            searchButton.onClick.AddListener(OpenUrl);
    }

    private void OpenUrl()
    {
        Application.OpenURL(url);
    }
}