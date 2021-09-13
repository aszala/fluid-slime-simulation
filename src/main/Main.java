package main;

import java.awt.Color;

import javax.swing.JFrame;

public class Main extends JFrame {

    private static final String TITLE = "Fluid Slime Simulation";
    public static final int WIDTH = 1280, HEIGHT = 720;

    public static final Color BACKGROUND_COLOR = Color.BLACK;

    public Main() {
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setTitle(TITLE);
        setLayout(null);
        setSize(WIDTH, HEIGHT);
        setLocationRelativeTo(null);
        getContentPane().setBackground(BACKGROUND_COLOR);
        setVisible(true);
    }

    public static void main(String[] args) {
		System.setProperty("sun.java2d.opengl", "true");

        Main gui = new Main();
        Renderer renderer = new Renderer();

        gui.getContentPane().add(renderer);

        for (int i=0;i<1000;i++) {
            Renderer.spawnNewAgent(Main.WIDTH/2, Main.HEIGHT/2);
        }

        Thread renderLoop = new Thread(new Runnable(){
            @Override
            public void run() {
                while (true) {
                    renderer.repaint();
                    renderer.update();

                    try {    
                        Thread.sleep(1000/60);
                    } catch (Exception e) {
                        System.err.println("Thread error");
                    }
                }
            }
        });
        renderLoop.start();
    }
}
