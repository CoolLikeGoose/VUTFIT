import asyncio
import snmpsim.commands.responder as responder

if __name__ == '__main__':
    asyncio.set_event_loop(asyncio.new_event_loop())
    responder.main()
