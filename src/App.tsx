import { Routes, Route, Navigate } from 'react-router-dom';
import { Navigation } from './components/Navigation';
import { Home } from './pages/Home';
import { ProjetoDetalhe } from './pages/ProjetoDetalhe';
import { DicionarioPage } from './pages/DicionarioPage';
import { TrilhaPage } from './pages/TrilhaPage';
import { SobrePage } from './pages/SobrePage';
import { useLocalStorage } from './hooks/useLocalStorage';
import { PROJETOS_EXPLICADOS } from './content/projetosData';

export default function App() {
  const [entendidos] = useLocalStorage<number[]>('codigos-entendidos', []);
  const total = PROJETOS_EXPLICADOS.length;

  return (
    <div data-app="codigo-explicado" className="min-h-screen bg-bench text-ink flex flex-col transition-colors selection:bg-violet-500/20">
      <Navigation progressCount={entendidos.length} totalCount={total} />
      <main className="flex-1">
        <Routes>
          <Route path="/" element={<Home />} />
          <Route path="/projeto/:numero" element={<ProjetoDetalhe />} />
          <Route path="/dicionario" element={<DicionarioPage />} />
          <Route path="/trilha" element={<TrilhaPage />} />
          <Route path="/sobre" element={<SobrePage />} />
          <Route path="*" element={<Navigate to="/" replace />} />
        </Routes>
      </main>
    </div>
  );
}
