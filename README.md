# Win32k Denial of Service Vulnerability

Uninitilialized buffer, which is null, in `CManipulationMarshaler` object is dereferenced when `NtDCompositionCommitChannel` is called.

## History

- 2023.05.11. Initial report to MSRC
- 2023.05.13. Status changed to `Review / Repro`
- 2023.06.16. Status changed from `Review / Repro` to `Complete`

Status is `Complete` on MSRC Researcher Portal, which means that the bug has been fixed. But still not fixed.
