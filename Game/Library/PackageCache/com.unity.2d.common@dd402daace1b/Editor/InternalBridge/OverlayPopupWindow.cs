using System;
using UnityEditor.Overlays;
using UnityEngine;
using UnityEngine.UIElements;

namespace UnityEditor.U2D.Common
{
    internal class OverlayPopupWindow : UnityEditor.Overlays.OverlayPopupWindow
    {
        public new static T Show<T>(VisualElement trigger, Vector2 size) where T : EditorWindow
        {
            return PopupWindowBase.Show<T>(GUIUtility.GUIToScreenRect(trigger.worldBound), size);
        }
    }
}