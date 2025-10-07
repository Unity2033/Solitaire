namespace UnityEditor.U2D.Common
{
    internal static class AssetCreationUtility
    {
        public static void CreateAssetObjectFromTemplate<T>(string sourcePath) where T : UnityEngine.Object
        {
            ItemCreationUtility.CreateAssetObjectFromTemplate<T>(sourcePath);
        }
    }
}
