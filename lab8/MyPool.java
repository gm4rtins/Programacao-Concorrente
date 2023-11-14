/* Disciplina: Programacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 8 */
/* Codigo: Criando um pool de threads em Java */

import java.util.LinkedList;
import java.util.concurrent.atomic.AtomicInteger;
import java.lang.Math; 


//-------------------------------------------------------------------------------
//Classe que define um pool de threads 
//!!! Documentar essa classe !!!
class FilaTarefas {
    private final int nThreads;
    private final MyPoolThreads[] threads;
    private final LinkedList<Runnable> queue;
    private boolean shutdown;

    public FilaTarefas(int nThreads) {
        this.shutdown = false;
        this.nThreads = nThreads;
        queue = new LinkedList<Runnable>();
        threads = new MyPoolThreads[nThreads];
        for (int i=0; i<nThreads; i++) {
            threads[i] = new MyPoolThreads();
            threads[i].start();
        } 
    }

    public void execute(Runnable r) {
        synchronized(queue) {
            if (this.shutdown) return;
            queue.addLast(r);
            queue.notify();
        }
    }
    
    public void shutdown() {
        synchronized(queue) {
            this.shutdown=true;
            queue.notifyAll();
        }
        for (int i=0; i<nThreads; i++) {
          try { threads[i].join(); } catch (InterruptedException e) { return; }
        }
    }

    private class MyPoolThreads extends Thread {
       public void run() {
         Runnable r;
         while (true) {
           synchronized(queue) {
             while (queue.isEmpty() && (!shutdown)) {
               try { queue.wait(); }
               catch (InterruptedException ignored){}
             }
             if (queue.isEmpty() && shutdown) return;  
             r = (Runnable) queue.removeFirst();
           }
           try { r.run(); }
           catch (RuntimeException e) {}
         } 
       } 
    } 
}
//-------------------------------------------------------------------------------

//--PASSO 1: cria uma classe que implementa a interface Runnable 
class TestePrimo implements Runnable {
   int numero;
   AtomicInteger nPrimos;

    public TestePrimo(int num, AtomicInteger nPrimos) { 
      this.numero = num; 
      this.nPrimos = nPrimos;
  }

    private int ehPrimo(int n) {
      int i;
      if (n<=1) return 0;
      if (n==2) return 1;
      if (n%2==0) return 0;
      for (i=3; i<Math.sqrt(n)+1; i+=2)
      if(n%i==0) return 0;
      return 1;
    }

   //--metodo executado pela thread
   public void run() {
      if( ehPrimo(numero) == 1){
        nPrimos.getAndIncrement(); //incrementa se for primo
      }  
   }
}

//Classe da aplicação (método main)
class MyPool {
    private static final int NTHREADS = 10;
    
    public static void main (String[] args) {
      //--PASSO 2: cria o pool de threads
      FilaTarefas pool = new FilaTarefas(NTHREADS); 
      AtomicInteger nPrimos = new AtomicInteger(0);

      
      //--PASSO 3: dispara a execução dos objetos runnable usando o pool de threads
      for (int i = 0; i < 400; i++) {
        Runnable r = new TestePrimo(i, nPrimos);
        pool.execute(r);
      }

      //--PASSO 4: esperar pelo termino das threads
      pool.shutdown();
      System.out.println("Total de primos: " + nPrimos);
   }
}
