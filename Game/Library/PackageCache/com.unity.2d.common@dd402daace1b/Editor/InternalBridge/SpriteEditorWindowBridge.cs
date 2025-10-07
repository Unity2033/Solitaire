using System.Collections.Generic;
using UnityEditor.U2D.Sprites;

namespace UnityEditor.U2D.Common
{
    internal static class SpriteEditorWindowBridge
    {
        public static EditorWindow GetSpriteEditorWindow()
        {
            return EditorWindow.GetWindow<SpriteEditorWindow>();
        }

        public static List<SpriteEditorModuleBase> GetSpriteEditorModules(EditorWindow window)
        {
            if (window is SpriteEditorWindow sew)
            {
                return sew.activatedModules;
            }
            return null;
        }

        public static void ActivateModule(EditorWindow window, int i)
        {
            if (window is SpriteEditorWindow sew)
            {
                sew.SetupModule(i);
            }
        }

        public static List<object> GetSpriteModuleModes(SpriteEditorModuleBase module)
        {
            var ret = new List<object>();
            var modes = (module as SpriteEditorModuleModeSupportBase)?.modes;
            if(modes != null)
                ret.AddRange(modes);
            return ret;
        }
    }
}