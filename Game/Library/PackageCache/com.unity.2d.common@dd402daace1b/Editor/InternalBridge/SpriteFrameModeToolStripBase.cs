#if ENABLE_SPRITEMODULE_MODE
using System;
using UnityEditor.Overlays;
using UnityEditor.U2D.Sprites;
using UnityEngine.UIElements;

namespace UnityEditor.U2D.Common
{
    abstract class SpriteFrameModeToolStripBase : UnityEditor.U2D.Sprites.Overlay.SpriteFrameModeToolStripBase
    {
        protected abstract bool SpriteFrameModeToggled(SpriteFrameModeToolStripBase value);
        public override void NotifyModeToolStripToggled(UnityEditor.U2D.Sprites.Overlay.SpriteFrameModeToolStripBase value)
        {
            SpriteFrameModeToggled(value as SpriteFrameModeToolStripBase);
        }

        protected void ActivateSpriteFrameModeTool()
        {
            base.OnToolStripToggled();
        }

        public override VisualElement[] GetUIContent(Layout overlayLayout)
        {
            return null;
        }

        public override int order { get; }

        public override bool OverlayActivated(ISpriteEditorModuleMode spriteEditor)
        {
            return OverlayActivated(spriteEditor as SpriteEditorFrameModuleModeBase);
        }

        public abstract bool OverlayActivated(SpriteEditorFrameModuleModeBase spriteEditor);

        public override void OverlayDeactivated() { }

        public override Type GetSpriteFrameModeType()
        {
            return default;
        }
    }
}
#endif
