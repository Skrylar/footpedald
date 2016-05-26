# FootpedalD
A tiny, reliable daemon for translating between a VEC Infinity Pedal and X11.

## Configuration
Configuration is performed through environment variables^[http://12factor.net/config].

| Variable  | Purpose                                       |
|-----------+-----------------------------------------------|
| FOOTPEDAL | Path to your footpedal (ex. /dev/usb/hiddev1) |

To start the daemon directly, use something like:

```sh
$ FOOTPEDAL="/dev/usb/hiddev1" ./footpedald
```

# Design Goals
`footpedald` is designed to be both long-running and as small as possible. Any intelligent decisions (such as what pedal driver to use, which device, what to do when buttons are pressed) are deferred to either the environment or a separate setup process.

# TODO
 - Allow changing the buttons mapped to pedal keys.
 - Fork in to the background.
 - Friendlier CLI front end for the daemon.
 - IPC to talk with GUI frontends.
 - GUI (GTK indicator/elementary switchboard) user-friendly front end.
