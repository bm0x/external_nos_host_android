# Android components for Nugget

Android communicates with Nugget apps in order to implement security related
HALs. Currently, those HALs are Keymaster, Weaver and OemLock.

### Services

Apps that define a protobuf service will have an app interface class
autogenerated. These classes will wrap a `NuggetClient`. The generator can be
found in the `generator` directory.

### Asynchronous communication

Work in progress.

Currently, everything is synchronous and just exposes the `call_application()`
function from the Nugget transport API. In future, asynchronous calls may be
desired. Support for this could be added in:

   * Nugget transport API
   * `NuggetClient` on top of `call_application()`
   * Generated services

## citadeld

Citadel will be running Nugget. In order to synchronize access to the driver /
bus, HALs should proxy all communication via the `citadeld` daemon which will be
the only service with driver access.

Synchronizing with this service, rather than in the driver, allows for easier
debugging and fixing should the need arise.

`CitadeldProxyClient` will implement `NuggetClient` to handle proxying
communication via `citadeld` without requiring change to the HALs.
