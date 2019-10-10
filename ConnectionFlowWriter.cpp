#include <ConnectionFlowWriter.h>
#include <functional>

namespace lima {

    // TODO: create adding to flowmap logic - includes maxSIZE
    void ConnectionFlowWriter::packetProcess() {
        while (!mProcessPackets.empty()) {
            auto packet = mProcessPackets.back();
            ConnectionFlow newFlow = ConnectionFlow(packet);
            
            // najpierw sprawdzam czy just jest taki src/dst ip jesli nie to dodaje nowego flowa
            // jesli jest to:
            // sprawdzam czy czas od ostatniej aktualki jest wiekszy niz 15 sek lub
            // jesli jest zakonczony przez flage fin/rst pakietu to tworze nowego flowa a tego zakonczonego przenosze do readyToExportMap;
            // jesli mapa przekroczy MAXSIZE to przenoszę calą do ready to eksport.
            
            ConnectionFlow flow = ConnectionFlow(packet);
            mProcessPackets.pop();
        }   
    }

    void ConnectionFlowWriter::start() {
        mWork = true;
        
        while (mWork) {
            std::unique_lock<std::mutex> mlock(mReceicePacketsMutex);
            mReceiveCondVar.wait(mlock, std::bind(ConnectionFlowWriter::isQueueReady, this));
            mProcessPackets = PacketQueue(std::move(mReceivePackets));
            packetProcess();
        }    
    }

    void ConnectionFlowWriter::stop() {
        mWork = false;
    }
}