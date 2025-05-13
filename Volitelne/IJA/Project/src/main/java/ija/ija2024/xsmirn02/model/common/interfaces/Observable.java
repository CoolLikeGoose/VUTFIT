package ija.ija2024.xsmirn02.model.common.interfaces;
//Author : Smirnov Nikita : xsmirn02. Observable patter implementation
public interface Observable {
    Observer observer = null;

    void registerObserver(Observer var1);
    void removeObserver(Observer var1);

    void notifyObserver();

    public interface Observer {
        void update(Observable var1);
    }
}
