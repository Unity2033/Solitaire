using System.Collections.Generic;
using UnityEditor.U2D.Sprites;

namespace UnityEditor.U2D.Common
{
    internal class SpriteCustomDataProvider : ISpriteCustomDataProvider
    {
        public static bool HasDataProvider(ISpriteEditorDataProvider dataProvider)
        {
            return dataProvider.HasDataProvider(typeof(ISpriteCustomDataProvider));
        }

        readonly ISpriteCustomDataProvider m_DataProvider;

        public SpriteCustomDataProvider(ISpriteEditorDataProvider dataProvider)
        {
            m_DataProvider = dataProvider.GetDataProvider<ISpriteCustomDataProvider>();
        }

        public IEnumerable<string> GetKeys() => m_DataProvider.GetKeys();

        public void SetData(string key, string data) => m_DataProvider.SetData(key, data);

        public void RemoveData(string key) => m_DataProvider.RemoveData(key);

        public bool GetData(string key, out string data) => m_DataProvider.GetData(key, out data);
    }
}
