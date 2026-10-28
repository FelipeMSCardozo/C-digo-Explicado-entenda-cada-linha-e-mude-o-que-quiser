import { useState, useEffect, useCallback } from 'react';

export function useLocalStorage<T>(key: string, initialValue: T): [T, (value: T | ((prev: T) => T)) => void] {
  const [storedValue, setStoredValue] = useState<T>(() => {
    try {
      if (typeof window === 'undefined') return initialValue;
      const item = window.localStorage.getItem(key);
      return item ? (JSON.parse(item) as T) : initialValue;
    } catch {
      return initialValue;
    }
  });

  const setValue = useCallback(
    (value: T | ((prev: T) => T)) => {
      try {
        setStoredValue((prev) => {
          const valueToStore = value instanceof Function ? value(prev) : value;
          try {
            if (typeof window !== 'undefined') {
              window.localStorage.setItem(key, JSON.stringify(valueToStore));
            }
          } catch {
            // Silently ignore quota or access errors
          }
          return valueToStore;
        });
      } catch {
        // Silently ignore state update errors
      }
    },
    [key]
  );

  useEffect(() => {
    try {
      const handleStorage = (e: StorageEvent) => {
        if (e.key === key && e.newValue) {
          try {
            setStoredValue(JSON.parse(e.newValue) as T);
          } catch {
            // Ignore invalid JSON
          }
        }
      };
      window.addEventListener('storage', handleStorage);
      return () => window.removeEventListener('storage', handleStorage);
    } catch {
      return undefined;
    }
  }, [key]);

  return [storedValue, setValue];
}
