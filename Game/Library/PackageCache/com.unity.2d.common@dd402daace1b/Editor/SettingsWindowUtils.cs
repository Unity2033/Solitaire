using UnityEngine;

namespace UnityEditor.U2D.Common
{
    internal class SettingsWindowUtils
    {
        internal class GUIScope : GUI.Scope
        {
            static float s_DefaultLabelWidth = 250.0f;
            static float s_DefaultLayoutMaxWidth = 500.0f;
            static float s_marginLeft = 10.0f;
            static float s_marginTop = 10.0f;
            float m_LabelWidth;

            public GUIScope(float layoutMaxWidth)
            {
                m_LabelWidth = EditorGUIUtility.labelWidth;
                EditorGUIUtility.labelWidth = s_DefaultLabelWidth;
                GUILayout.BeginHorizontal();
                GUILayout.Space(s_marginLeft);
                GUILayout.BeginVertical();
                GUILayout.Space(s_marginTop);
            }

            public GUIScope() : this(s_DefaultLayoutMaxWidth)
            {
            }

            protected override void CloseScope()
            {
                GUILayout.EndVertical();
                GUILayout.EndHorizontal();
                EditorGUIUtility.labelWidth = m_LabelWidth;
            }
        }
    }
}
