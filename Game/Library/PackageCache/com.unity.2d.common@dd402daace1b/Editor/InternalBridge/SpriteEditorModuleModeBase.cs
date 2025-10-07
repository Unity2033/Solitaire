#if ENABLE_SPRITEMODULE_MODE
using System;
using System.Collections.Generic;
using UnityEditor.U2D.Sprites;
using UnityEngine;

namespace UnityEditor.U2D.Common
{
    [SpriteEditorModuleMode(types: new[] { typeof(SpriteFrameModule) })]
    internal abstract class SpriteEditorFrameModuleModeBase : ISpriteEditorModuleMode
    {
        ISpriteEditor m_SpriteEditor;
        event Action<ISpriteEditorModuleMode> m_ModeActivateCallback = _ => { };
        event Action<SpriteRect> m_SpriteEditorSpriteSelectionChanged = _ => { };
        SpriteEditorModuleModeSupportBase m_Module;

        public SpriteEditorModuleBase module => m_Module;

        public abstract bool ActivateMode();

        public abstract void DeactivateMode();
        public void OnAddToModule(UnityEditor.U2D.Sprites.SpriteEditorModuleModeSupportBase module)
        {
            m_Module = module;
            spriteEditor.GetMainVisualContainer().RegisterCallback<SpriteSelectionChangeEvent>(OnSpriteEditorSpriteSelectionChanged);
            OnAddToModuleInternal(module);
        }

        protected abstract void OnAddToModuleInternal(SpriteEditorModuleBase module);

        public void OnRemoveFromModule(UnityEditor.U2D.Sprites.SpriteEditorModuleModeSupportBase module)
        {
            if (m_Module == module)
            {
                spriteEditor.GetMainVisualContainer().UnregisterCallback<SpriteSelectionChangeEvent>(OnSpriteEditorSpriteSelectionChanged);
                OnRemoveFromModuleInternal(module);
                m_Module = null;
            }
        }

        public event Action<ISpriteEditorModuleMode> onModeRequestActivate
        {
            add => m_ModeActivateCallback += value;
            remove => m_ModeActivateCallback += value;
        }

        protected abstract void OnRemoveFromModuleInternal(SpriteEditorModuleBase module);

        void OnSpriteEditorSpriteSelectionChanged(SpriteSelectionChangeEvent evt)
        {
            m_SpriteEditorSpriteSelectionChanged?.Invoke(spriteEditor.selectedSpriteRect);
        }

        public void RequestModeToActivate(ISpriteEditorModuleMode moduleMode)
        {
            m_ModeActivateCallback?.Invoke(moduleMode);
        }

        public abstract bool ApplyModeData(bool apply, HashSet<Type> dataProviderTypes);

        public void RequestModeToActivate()
        {
            RequestModeToActivate(this);
        }

        public ISpriteEditor spriteEditor
        {
            get => m_SpriteEditor;
            set => m_SpriteEditor = value;
        }

        public abstract bool CanBeActivated();

        public abstract void DoMainGUI();

        public abstract void DoToolbarGUI(Rect drawArea);

        public abstract void DoPostGUI();
    }
}
#endif
