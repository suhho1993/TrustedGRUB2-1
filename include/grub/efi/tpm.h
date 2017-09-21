#include <grub/efi/api.h>

#define EFI_TPM_GUID {0xf541796d, 0xa62e, 0x4954, {0xa7, 0x75, 0x95, 0x84, 0xf6, 0x1b, 0x9c, 0xdd }};
#define EFI_TPM2_GUID {0x607f766c, 0x7455, 0x42be, {0x93, 0x0b, 0xe4, 0xd7, 0x6d, 0xb2, 0x72, 0x0f }};
#define EFIAPI

#ifndef TPM_BOOLEAN
typedef enum { false = 0, true = 1 } BOOLEAN;
#define TPM_BOOLEAN
#endif
/*modified for grub*/
grub_efi_status_t tpm_log_event(grub_addr_t buf, grub_uint64_t size, grub_uint8_t pcr,
			 const unsigned char *description);
//EFI_STATUS tpm_log_event(EFI_PHYSICAL_ADDRESS buf, UINTN size, UINT8 pcr,
//			 const CHAR8 *description);


/************************* constants *************************/

#define EFI_SUCCESS	0

/* Command return codes */
#define TPM_BASE 0x0
#define TPM_SUCCESS TPM_BASE
#define TPM_AUTHFAIL (TPM_BASE + 0x1)
#define TPM_BADINDEX (TPM_BASE + 0x2)


/* TODO: 0x10000 does not work for some reason */
/* is  0x20000 and 0x30000 a good choice? */

#define INPUT_PARAM_BLK_ADDR 0x30000
#define OUTPUT_PARAM_BLK_ADDR 0x20000

#define TPM_TAG_RQU_COMMAND 0x00C1

#define TCPA 0x41504354
/************************* macros *************************/

#define CHECK_FOR_NULL_ARGUMENT( argument )                                                                 \
                        if( ! argument ) {                                                                                                  \
                                grub_fatal( "BAD_ARGUMENT: argument is NULL" );                                 \
                        }

#ifndef GRUB_EFI_TPM_HEADER
#define GRUB_EFI_TPM_HEADER 1

typedef struct {
  grub_efi_uint8_t Major;
  grub_efi_uint8_t Minor;
  grub_efi_uint8_t RevMajor;
  grub_efi_uint8_t RevMinor;
} TCG_VERSION;

typedef struct _TCG_EFI_BOOT_SERVICE_CAPABILITY {
  grub_efi_uint8_t          Size;                /// Size of this structure.
  TCG_VERSION    StructureVersion;
  TCG_VERSION    ProtocolSpecVersion;
  grub_efi_uint8_t          HashAlgorithmBitmap; /// Hash algorithms .
  char        TPMPresentFlag;      /// 00h = TPM not present.
  char        TPMDeactivatedFlag;  /// 01h = TPM currently deactivated.
} TCG_EFI_BOOT_SERVICE_CAPABILITY;

typedef struct {
  grub_efi_uint32_t PCRIndex;
  grub_efi_uint32_t EventType;
  grub_efi_uint8_t digest[20];
  grub_efi_uint32_t EventSize;
  grub_efi_uint8_t  Event[1];
} TCG_PCR_EVENT;

struct grub_efi_tpm_protocol
{
  grub_efi_status_t (*status_check) (struct grub_efi_tpm_protocol *this,
				     TCG_EFI_BOOT_SERVICE_CAPABILITY *ProtocolCapability,
				     grub_efi_uint32_t *TCGFeatureFlags,
				     grub_efi_physical_address_t *EventLogLocation,
				     grub_efi_physical_address_t *EventLogLastEntry);
  grub_efi_status_t (*hash_all) (struct grub_efi_tpm_protocol *this,
				 grub_efi_uint8_t *HashData,
				 grub_efi_uint64_t HashLen,
				 grub_efi_uint32_t AlgorithmId,
				 grub_efi_uint64_t *HashedDataLen,
				 grub_efi_uint8_t **HashedDataResult);
  grub_efi_status_t (*log_event) (struct grub_efi_tpm_protocol *this,
				  TCG_PCR_EVENT *TCGLogData,
				  grub_efi_uint32_t *EventNumber,
				  grub_efi_uint32_t Flags);
  grub_efi_status_t (*pass_through_to_tpm) (struct grub_efi_tpm_protocol *this,
					    grub_efi_uint32_t TpmInputParameterBlockSize,
					    grub_efi_uint8_t *TpmInputParameterBlock,
					    grub_efi_uint32_t TpmOutputParameterBlockSize,
					    grub_efi_uint8_t *TpmOutputParameterBlock);
  grub_efi_status_t (*log_extend_event) (struct grub_efi_tpm_protocol *this,
					 grub_efi_physical_address_t HashData,
					 grub_efi_uint64_t HashDataLen,
					 grub_efi_uint32_t AlgorithmId,
					 TCG_PCR_EVENT *TCGLogData,
					 grub_efi_uint32_t *EventNumber,
					 grub_efi_physical_address_t *EventLogLastEntry);
};

typedef struct grub_efi_tpm_protocol grub_efi_tpm_protocol_t;

typedef grub_efi_uint32_t EFI_TCG2_EVENT_LOG_BITMAP;
typedef grub_efi_uint32_t EFI_TCG2_EVENT_LOG_FORMAT;
typedef grub_efi_uint32_t EFI_TCG2_EVENT_ALGORITHM_BITMAP;

typedef struct tdEFI_TCG2_VERSION {
  grub_efi_uint8_t Major;
  grub_efi_uint8_t Minor;
} GRUB_PACKED EFI_TCG2_VERSION;

typedef struct tdEFI_TCG2_BOOT_SERVICE_CAPABILITY {
  grub_efi_uint8_t Size;
  EFI_TCG2_VERSION StructureVersion;
  EFI_TCG2_VERSION ProtocolVersion;
  EFI_TCG2_EVENT_ALGORITHM_BITMAP HashAlgorithmBitmap;
  EFI_TCG2_EVENT_LOG_BITMAP SupportedEventLogs;
  grub_efi_boolean_t TPMPresentFlag;
  grub_efi_uint16_t MaxCommandSize;
  grub_efi_uint16_t MaxResponseSize;
  grub_efi_uint32_t ManufacturerID;
  grub_efi_uint32_t NumberOfPcrBanks;
  EFI_TCG2_EVENT_ALGORITHM_BITMAP ActivePcrBanks;
} EFI_TCG2_BOOT_SERVICE_CAPABILITY;

typedef grub_efi_uint32_t TCG_PCRINDEX;
typedef grub_efi_uint32_t TCG_EVENTTYPE;

typedef struct tdEFI_TCG2_EVENT_HEADER {
  grub_efi_uint32_t HeaderSize;
  grub_efi_uint16_t HeaderVersion;
  TCG_PCRINDEX PCRIndex;
  TCG_EVENTTYPE EventType;
} GRUB_PACKED EFI_TCG2_EVENT_HEADER;

typedef struct tdEFI_TCG2_EVENT {
  grub_efi_uint32_t Size;
  EFI_TCG2_EVENT_HEADER Header;
  grub_efi_uint8_t Event[1];
} GRUB_PACKED EFI_TCG2_EVENT;

struct grub_efi_tpm2_protocol
{
  grub_efi_status_t (*get_capability) (struct grub_efi_tpm2_protocol *this,
				       EFI_TCG2_BOOT_SERVICE_CAPABILITY *ProtocolCapability);
  grub_efi_status_t (*get_event_log) (struct grub_efi_tpm2_protocol *this,
				      EFI_TCG2_EVENT_LOG_FORMAT EventLogFormat,
				      grub_efi_physical_address_t *EventLogLocation,
				      grub_efi_physical_address_t *EventLogLastEntry,
				      grub_efi_boolean_t *EventLogTruncated);
  grub_efi_status_t (*hash_log_extend_event) (struct grub_efi_tpm2_protocol *this,
					      grub_efi_uint64_t Flags,
					      grub_efi_physical_address_t *DataToHash,
					      grub_efi_uint64_t DataToHashLen,
					      EFI_TCG2_EVENT *EfiTcgEvent);
  grub_efi_status_t (*submit_command) (struct grub_efi_tpm2_protocol *this,
				       grub_efi_uint32_t InputParameterBlockSize,
				       grub_efi_uint8_t *InputParameterBlock,
				       grub_efi_uint32_t OutputParameterBlockSize,
				       grub_efi_uint8_t *OutputParameterBlock);
  grub_efi_status_t (*get_active_pcr_blanks) (struct grub_efi_tpm2_protocol *this,
					      grub_efi_uint32_t *ActivePcrBanks);
  grub_efi_status_t (*set_active_pcr_banks) (struct grub_efi_tpm2_protocol *this,
					     grub_efi_uint32_t ActivePcrBanks);
  grub_efi_status_t (*get_result_of_set_active_pcr_banks) (struct grub_efi_tpm2_protocol *this,
							   grub_efi_uint32_t *OperationPresent,
							   grub_efi_uint32_t *Response);
};

typedef struct grub_efi_tpm2_protocol grub_efi_tpm2_protocol_t;

#define TCG_ALG_SHA 0x00000004

#endif
>>>>>>> 6782f6d431d22b4e9ab14e94d263795c7991e160
