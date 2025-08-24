# Patches Required for Compilation

This document describes the patches that may be required to successfully build the PicoSwitch-WirelessGamepadAdapter project.

## BTStack API Compatibility Patch

### Problem
The Bluepad32 submodule uses an older version of the BTStack library API. Recent versions of BTStack changed the structure of `btstack_hid_parser_t`, moving some global fields into a nested `usage_iterator` structure.

### Symptoms
If you see compilation errors like:
```
error: 'btstack_hid_parser_t' has no member named 'global_logical_minimum'
error: 'btstack_hid_parser_t' has no member named 'global_logical_maximum'
```

### Solution
Apply the BTStack compatibility patch to fix the structure member access.

### Affected File
- `bluepad32/src/components/bluepad32/parser/uni_hid_parser.c`

### How to Apply

#### Automatic (Recommended)
Run the provided script:
```bash
./apply-btstack-patch.sh
```

#### Manual
Edit the file `bluepad32/src/components/bluepad32/parser/uni_hid_parser.c` and change lines 41-46:

**Before:**
```c
parser.global_logical_minimum = 0;
parser.global_logical_maximum = 0;
parser.global_report_count = 0;
parser.global_report_id = 0;
parser.global_report_size = 0;
parser.global_usage_page = 0;
```

**After:**
```c
parser.usage_iterator.global_logical_minimum = 0;
parser.usage_iterator.global_logical_maximum = 0;
parser.usage_iterator.global_report_count = 0;
parser.usage_iterator.global_report_id = 0;
parser.usage_iterator.global_report_size = 0;
parser.usage_iterator.global_usage_page = 0;
```

#### Using Git Patch
You can also apply the patch using git:
```bash
patch -p1 < patches/bluepad32-btstack-fix.patch
```

### Docker Builds
Docker builds automatically apply this patch, so no manual intervention is required.

## Future Considerations

### Updating Bluepad32
When updating the Bluepad32 submodule to a newer version, this patch may no longer be necessary if the upstream project fixes the BTStack compatibility issue.

### Alternative Solutions
- **Fork Bluepad32**: Create a fork of the Bluepad32 repository with the fix applied
- **Update BTStack**: Use an older version of BTStack that's compatible with the current Bluepad32 code
- **Upstream Fix**: Contribute the fix back to the Bluepad32 project

## Testing
After applying the patch, verify that the compilation works:
```bash
make build
```

The build should complete without errors related to BTStack structure members.
