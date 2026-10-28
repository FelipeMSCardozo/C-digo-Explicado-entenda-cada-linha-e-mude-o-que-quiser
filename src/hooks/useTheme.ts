import { useState, useEffect, useCallback } from 'react';

export type ThemeMode = 'dark' | 'light';

export function useTheme(storageKey = 'agy_arduino_theme') {
  const [theme, setTheme] = useState<ThemeMode>(() => {
    try {
      if (typeof window !== 'undefined') {
        const saved = window.localStorage.getItem(storageKey);
        if (saved === 'dark' || saved === 'light') return saved;
        if (window.matchMedia && window.matchMedia('(prefers-color-scheme: light)').matches) {
          return 'light';
        }
      }
    } catch {
      // ignore
    }
    return 'dark';
  });

  useEffect(() => {
    try {
      const root = document.documentElement;
      if (theme === 'dark') {
        root.classList.add('dark');
      } else {
        root.classList.remove('dark');
      }
      window.localStorage.setItem(storageKey, theme);
    } catch {
      // ignore
    }
  }, [theme, storageKey]);

  const toggleTheme = useCallback(() => {
    setTheme((prev) => (prev === 'dark' ? 'light' : 'dark'));
  }, []);

  return { theme, setTheme, toggleTheme };
}
